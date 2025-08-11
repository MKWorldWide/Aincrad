using UnityEditor;
using UnityEngine;
using System.Collections.Generic;
using System.IO;
using Debug = UnityEngine.Debug;

public class DependencyChecker : EditorWindow
{
    private Vector2 scrollPosition;
    private Dictionary<string, bool> dependencies = new Dictionary<string, bool>();
    private bool showFixOptions = false;
    private bool showInstallInstructions = false;
    private string vccPath = "";

    [MenuItem("VRChat/Check Dependencies")]
    public static void ShowWindow()
    {
        GetWindow<DependencyChecker>("VRChat Dependency Checker");
    }

    private void OnEnable()
    {
        CheckDependencies();
    }

    private void OnGUI()
    {
        GUILayout.Label("VRChat Dependency Checker", EditorStyles.boldLabel);
        EditorGUILayout.Space();

        if (GUILayout.Button("Check Dependencies"))
        {
            CheckDependencies();
        }

        EditorGUILayout.Space();
        scrollPosition = EditorGUILayout.BeginScrollView(scrollPosition);

        foreach (var dep in dependencies)
        {
            EditorGUILayout.BeginHorizontal();
            EditorGUILayout.LabelField(dep.Key, GUILayout.Width(250));
            GUIStyle style = new GUIStyle(EditorStyles.label);
            style.normal.textColor = dep.Value ? Color.green : Color.red;
            EditorGUILayout.LabelField(dep.Value ? "✓ Installed" : "✗ Missing", style);
            EditorGUILayout.EndHorizontal();
        }

        EditorGUILayout.EndScrollView();

        EditorGUILayout.Space();
        showFixOptions = EditorGUILayout.Foldout(showFixOptions, "Fix Options");
        if (showFixOptions)
        {
            EditorGUI.indentLevel++;
            
            if (GUILayout.Button("Install Missing Dependencies"))
            {
                InstallMissingDependencies();
            }

            EditorGUILayout.Space();
            
            EditorGUILayout.BeginHorizontal();
            vccPath = EditorGUILayout.TextField("VCC Path", vccPath);
            if (GUILayout.Button("Browse", GUILayout.Width(80)))
            {
                vccPath = EditorUtility.OpenFilePanel("Locate VRChat Creator Companion", "", "exe");
            }
            EditorGUILayout.EndHorizontal();

            if (GUILayout.Button("Open VCC"))
            {
                if (!string.IsNullOrEmpty(vccPath) && File.Exists(vccPath))
                {
                    System.Diagnostics.Process.Start(vccPath);
                }
                else
                {
                    Debug.LogError("VCC path is invalid. Please locate the VCC executable.");
                }
            }

            EditorGUI.indentLevel--;
        }

        EditorGUILayout.Space();
        showInstallInstructions = EditorGUILayout.Foldout(showInstallInstructions, "Installation Instructions");
        if (showInstallInstructions)
        {
            EditorGUILayout.HelpBox(
                "1. Download and install VRChat Creator Companion (VCC) from the VRChat website\n" +
                "2. Open VCC and log in with your VRChat account\n" +
                "3. Click 'New Project' and select this project folder\n" +
                "4. Add the following packages:\n" +
                "   - VRChat SDK3 - Worlds\n" +
                "   - UdonSharp\n" +
                "   - TextMeshPro\n" +
                "5. Click 'Install/Update' to install all dependencies",
                MessageType.Info);
        }
    }

    private void CheckDependencies()
    {
        dependencies.Clear();

        // Check for VRChat SDK
        bool hasVRCSDK = TypeExists("VRC.SDK3.Editor.VRC_SdkBuilder");
        dependencies.Add("VRChat SDK3 - Worlds", hasVRCSDK);

        // Check for UdonSharp
        bool hasUdonSharp = TypeExists("UdonSharp.UdonSharpMenu");
        dependencies.Add("UdonSharp", hasUdonSharp);

        // Check for TextMeshPro
        bool hasTMP = TypeExists("TMPro.TMP_Text");
        dependencies.Add("TextMeshPro", hasTMP);

        // Check for Unity Recorder (optional)
        bool hasRecorder = TypeExists("UnityEditor.Recorder.RecorderWindow");
        dependencies.Add("Unity Recorder (Optional)", hasRecorder);

        // Check for VCC
        bool hasVCC = File.Exists(Path.Combine(Application.dataPath, "..", "VCC_Data"));
        dependencies.Add("VRChat Creator Companion", hasVCC);
    }

    private bool TypeExists(string typeName)
    {
        return System.Type.GetType(typeName) != null;
    }

    private void InstallMissingDependencies()
    {
        bool allInstalled = true;
        foreach (var dep in dependencies)
        {
            if (!dep.Value)
            allInstalled = false;
        }

        if (allInstalled)
        {
            EditorUtility.DisplayDialog("Dependencies", "All required dependencies are already installed!", "OK");
            return;
        }

        if (EditorUtility.DisplayDialog("Install Dependencies", 
            "This will open the VRChat Creator Companion to install missing dependencies. Continue?", 
            "Yes", "Cancel"))
        {
            // Try to find VCC in common locations
            string vccExe = FindVCC();
            
            if (!string.IsNullOrEmpty(vccExe) && File.Exists(vccExe))
            {
                System.Diagnostics.Process.Start(vccExe);
                EditorUtility.DisplayDialog("VCC Launched", 
                    "VRChat Creator Companion has been launched. Please install the missing packages.", "OK");
            }
            else
            {
                if (EditorUtility.DisplayDialog("VCC Not Found", 
                    "Could not find VRChat Creator Companion. Would you like to download it?", "Yes", "No"))
                {
                    Application.OpenURL("https://vcc.docs.vrchat.com/vpm/install-vcc");
                }
            }
        }
    }

    private string FindVCC()
    {
        // Check common installation paths
        string[] possiblePaths = new string[]
        {
            Path.Combine(Environment.GetFolderPath(Environment.SpecialFolder.ProgramFiles), "VRChat Creator Companion\VRChat Creator Companion.exe"),
            Path.Combine(Environment.GetFolderPath(Environment.SpecialFolder.ProgramFilesX86), "VRChat Creator Companion\VRChat Creator Companion.exe"),
            Path.Combine(Environment.GetFolderPath(Environment.SpecialFolder.LocalApplicationData), "Programs\VRChat Creator Companion\VRChat Creator Companion.exe")
        };

        foreach (string path in possiblePaths)
        {
            if (File.Exists(path))
                return path;
        }

        return string.Empty;
    }
}
