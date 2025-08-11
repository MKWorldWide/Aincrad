Shader "Custom/PortalEffect"
{
    Properties
    {
        _MainTex ("Texture", 2D) = "white" {}
        _Color ("Color", Color) = (1,1,1,1)
        _DistortionStrength ("Distortion Strength", Range(0, 0.2)) = 0.1
        _DistortionSpeed ("Distortion Speed", Range(0, 2)) = 0.5
        _EmissionIntensity ("Emission Intensity", Range(0, 5)) = 1.0
        _Rotation ("Rotation", Range(0, 360)) = 0
        _FresnelPower ("Fresnel Power", Range(0, 10)) = 2.0
        _FresnelBias ("Fresnel Bias", Range(0, 1)) = 0.0
        _FresnelScale ("Fresnel Scale", Range(0, 1)) = 1.0
    }
    
    SubShader
    {
        Tags { "Queue"="Transparent" "RenderType"="Transparent" }
        LOD 100
        
        Blend SrcAlpha OneMinusSrcAlpha
        ZWrite Off
        Cull Off
        
        Pass
        {
            CGPROGRAM
            #pragma vertex vert
            #pragma fragment frag
            #pragma multi_compile_fog
            
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
                float4 vertex : SV_POSITION;
                float3 viewDir : TEXCOORD1;
                float3 worldNormal : TEXCOORD2;
                UNITY_FOG_COORDS(3)
            };
            
            sampler2D _MainTex;
            float4 _MainTex_ST;
            float4 _Color;
            float _DistortionStrength;
            float _DistortionSpeed;
            float _EmissionIntensity;
            float _Rotation;
            float _FresnelPower;
            float _FresnelBias;
            float _FresnelScale;
            
            v2f vert (appdata v)
            {
                v2f o;
                o.vertex = UnityObjectToClipPos(v.vertex);
                o.uv = TRANSFORM_TEX(v.uv, _MainTex);
                o.viewDir = normalize(WorldSpaceViewDir(v.vertex));
                o.worldNormal = UnityObjectToWorldNormal(v.normal);
                UNITY_TRANSFER_FOG(o,o.vertex);
                return o;
            }
            
            fixed4 frag (v2f i) : SV_Target
            {
                // Rotate UVs
                float2 center = float2(0.5, 0.5);
                float2 uv = i.uv - center;
                float angle = radians(_Rotation);
                float2x2 rot = float2x2(cos(angle), -sin(angle),
                                      sin(angle), cos(angle));
                uv = mul(rot, uv) + center;
                
                // Create distortion effect
                float time = _Time.y * _DistortionSpeed;
                float2 distortion = float2(
                    sin(time + uv.y * 10) * 0.01,
                    cos(time + uv.x * 10) * 0.01
                ) * _DistortionStrength;
                
                // Apply distortion to UVs
                float2 distortedUV = uv + distortion;
                
                // Sample texture with distortion
                fixed4 col = tex2D(_MainTex, distortedUV) * _Color;
                
                // Fresnel effect
                float fresnel = _FresnelBias + _FresnelScale * pow(1.0 - abs(dot(normalize(i.viewDir), normalize(i.worldNormal))), _FresnelPower);
                
                // Combine effects
                col.rgb *= _EmissionIntensity * fresnel;
                col.a = saturate(col.a * fresnel * 2);
                
                // Apply fog
                UNITY_APPLY_FOG(i.fogCoord, col);
                
                return col;
            }
            ENDCG
        }
    }
    
    FallBack "Transparent/Diffuse"
}
