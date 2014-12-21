using UnityEngine;
using System.Collections;


public class Blink : MonoBehaviour
{
	public float TextBlinkTime;
	
	private Color _textColorON;
	private Color _textColorOFF;
	private bool _fadingIn;
	private float _timer;
	private float _tmpDeltaTime;
	
	void Start ()
	{
		#if UNITY_EDITOR

		if(guiText == null)
		{
			Debug.LogError("The script Blink must be attached to a GUIText Game Object!");
			gameObject.SetActive(false); // Disable the Game Object
			return;
		}
		#endif	
		
	
		var textColor = guiText.color;
		_textColorON = new Color(textColor.r, textColor.g, textColor.b, 1);
		_textColorOFF = new Color(textColor.r, textColor.g, textColor.b, 0);
		
		_fadingIn = false;
	}
	
	void Update ()
	{
		#if UNITY_EDITOR

		var textColor = guiText.color;
		_textColorON = new Color(textColor.r, textColor.g, textColor.b, 1);
		_textColorOFF = new Color(textColor.r, textColor.g, textColor.b, 0);
		#endif	
		
		_tmpDeltaTime = (Time.time - _timer) / TextBlinkTime;
		

		if(_fadingIn)
		{

			guiText.color = Color.Lerp(_textColorOFF, _textColorON, _tmpDeltaTime);
		}
		else
		{

			guiText.color = Color.Lerp(_textColorON, _textColorOFF, _tmpDeltaTime);
		}
		
		if(_tmpDeltaTime > 1)
		{

			_fadingIn = !_fadingIn; 
			_timer = Time.time;
		}
	}
}