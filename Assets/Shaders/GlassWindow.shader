Shader "Aincrad/GlassWindow"
{
    Properties
    {
        _MainTex ("Base (RGB) Trans (A)", 2D) = "white" {}
        _Color ("Color", Color) = (1,1,1,0.5)
        _ReflectionStrength ("Reflection Strength", Range(0,1)) = 0.8
        _Refraction ("Refraction", Range(0, 0.1)) = 0.05
    }
    
    SubShader
    {
        Tags { "Queue"="Transparent" "RenderType"="Transparent" }
        LOD 100

        GrabPass {}

        Pass
        {
            CGPROGRAM
            #pragma vertex vert
            #pragma fragment frag
            #include "UnityCG.cginc"

            struct appdata
            {
                float4 vertex : POSITION;
                float2 uv : TEXCOORD0;
                float3 normal : NORMAL;
            };

            struct v2f
            {
                float2 uv : TEXCOORD0;
                float4 grabPos : TEXCOORD1;
                float4 vertex : SV_POSITION;
                float3 worldNormal : TEXCOORD2;
                float3 viewDir : TEXCOORD3;
            };

            sampler2D _MainTex;
            sampler2D _GrabTexture;
            float4 _MainTex_ST;
            float4 _Color;
            float _ReflectionStrength;
            float _Refraction;

            v2f vert (appdata v)
            {
                v2f o;
                o.vertex = UnityObjectToClipPos(v.vertex);
                o.uv = TRANSFORM_TEX(v.uv, _MainTex);
                o.grabPos = ComputeGrabScreenPos(o.vertex);
                o.worldNormal = UnityObjectToWorldNormal(v.normal);
                o.viewDir = normalize(WorldSpaceViewDir(v.vertex));
                return o;
            }

            fixed4 frag (v2f i) : SV_Target
            {
                // Sample the texture
                fixed4 col = tex2D(_MainTex, i.uv) * _Color;
                
                // Calculate reflection
                float3 reflectDir = reflect(-i.viewDir, i.worldNormal);
                float4 reflection = UNITY_SAMPLE_TEXCUBE_LOD(unity_SpecCube0, reflectDir, 0);
                
                // Apply refraction to grab pass
                float2 offset = i.worldNormal.xy * _Refraction;
                i.grabPos.xy += offset * i.grabPos.z;
                float3 refraction = tex2Dproj(_GrabTexture, UNITY_PROJ_COORD(i.grabPos)).rgb;
                
                // Combine reflection and refraction
                col.rgb = lerp(refraction, reflection.rgb, _ReflectionStrength) * col.a;
                
                return col;
            }
            ENDCG
        }
    }
    FallBack "Transparent/Diffuse"
}
