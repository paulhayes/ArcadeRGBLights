using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.IO.Ports;
using System.IO;
using System.Text.RegularExpressions;

[CreateAssetMenu]
public class SerialLights : ExternalData 
{
	[SerializeField]
	string windowsName;

	[SerializeField]
	string unixName;
	
	[SerializeField]
	int baudRate;

	SerialPort _serialPort;


	public void Connect()
	{		
		string name = null;
		name = GetPortName(unixName,windowsName);

		if( name == null ){
			Debug.LogWarning("Unable to connect to serial port, name not found");
			return;
		}
		Disconnect();
		
		_serialPort = new SerialPort();	
		_serialPort.PortName = name;
		_serialPort.BaudRate = baudRate;
		_serialPort.DtrEnable = true;

		try {
			_serialPort.Open();
		}
		catch(System.Exception e){
			Debug.Log(e);
			_serialPort = null;
		}


	}

	public void Disconnect()
	{	
		if(_serialPort!=null){
			if( _serialPort.IsOpen ){
				_serialPort.Close();
			}
			_serialPort.Dispose();
			_serialPort = null;
		}
	}

	public void SetLight(int lightIndex, int color)
	{

		if( _serialPort == null )
			return;
		
		string msg = string.Format( "{0} {1:x6}\r",lightIndex,color );
		Debug.Log(msg);
		_serialPort.Write( msg );
		//_serialPort.ReadLine();
	}

	public static string GetPortName (string unixName,string windowsName)
	{
		int p = (int)System.Environment.OSVersion.Platform;
		string pattern;
		//List<string> serialPorts = new List<string> ();
		
		// Are we on Unix?
		if (p == 4 || p == 128 || p == 6) {
			pattern = unixName;
			string[] ttys = Directory.GetFiles ("/dev/", pattern);
			if(ttys.Length>0)
				return ttys[0];
		}
		else {
			pattern = windowsName;
			string[] ttys = SerialPort.GetPortNames();
			foreach (string dev in ttys) {
				if (Regex.IsMatch(dev, pattern)){
					return dev;
				}
				
			}
		}
		
		return null;
		
	}
}
