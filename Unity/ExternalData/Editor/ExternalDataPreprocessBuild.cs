using UnityEngine;
using UnityEditor;
using UnityEditor.Build;
using System.IO;

public class ExternalDataPreprocessBuild : IPostprocessBuild
{
    public int callbackOrder { get { return 0; } }

    public void OnPostprocessBuild(BuildTarget target, string path)
    {
        var settingsObjects = Resources.FindObjectsOfTypeAll<ExternalData>();

		foreach(var settings in settingsObjects){
			string dataDir = Path.Combine( Directory.GetParent(path).FullName,Path.GetFileNameWithoutExtension(path)+"_Data");
			Debug.LogFormat("{0} {1}",dataDir,Directory.Exists(dataDir)?"exists":"doesn't exist");
			string filePath = Path.Combine(dataDir,settings.GetFileName());
			File.WriteAllText( filePath, JsonUtility.ToJson( settings, true ) );

		}
    }

}