using UnityEngine;
using System.Collections;

public class GameOverScript3 : MonoBehaviour
{

	void OnGUI()
	{
		const int buttonWidth = 120;
		const int buttonHeight = 60;
		
		if (
			GUI.Button(

			new Rect(
			Screen.width / 2 - (buttonWidth / 2),
			( Screen.height / 2) - (buttonHeight / 2),
			buttonWidth,
			buttonHeight
			),
			"Retry!"
			)
			)
		{

			Application.LoadLevel("scenelvl3");
		}
		
		if (
			GUI.Button(

			new Rect(
			Screen.width / 2 - (buttonWidth / 2),
			(3 * Screen.height / 4) - (buttonHeight / 2),
			buttonWidth,
			buttonHeight
			),
			"Back to menu"
			)
			)
		{

			Application.LoadLevel("MenuPrincipal");
		}
	}
}

