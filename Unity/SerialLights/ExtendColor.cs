using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public static class ExtendColor 
{

	public static int ToInt(this Color32 color)
	{
		return (((color.r&0xff)<<16))|(((color.g&0xff)<<8))|(((color.b&0xff)<<0));
	}
}
