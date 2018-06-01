using System.IO;
using UnityEngine;

public abstract class ExternalData : ScriptableObject 
{
    void OnEnable()
    {
      string filePath = Path.Combine(Application.dataPath, GetFileName());
      if( File.Exists(filePath) ){
          JsonUtility.FromJsonOverwrite( File.ReadAllText(filePath), this );
      } 
    }

    public string GetFileName(){
      return name+".json";
    }
}