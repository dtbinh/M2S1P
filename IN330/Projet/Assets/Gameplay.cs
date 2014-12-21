using UnityEngine;
using System.Collections;

public class Gameplay : MonoBehaviour
{

	void OnGUI()
	{
		const int buttonWidth = 120;
		const int buttonHeight = 100;
		
		if (
			GUI.Button(

			new Rect(
			Screen.width / 3 - (buttonWidth / 2),
			(3 * Screen.height / 4) - (buttonHeight / 2),
			buttonWidth,
			buttonHeight
			),
			"Play"
			)
			)
		{

			Application.LoadLevel("chargementscene0");
		}
		
		if (
			GUI.Button(

			new Rect(
			(Screen.width -250) - (buttonWidth / 2),
			(3 * Screen.height / 4) - (buttonHeight / 2),
			buttonWidth,
			buttonHeight
			),
			"Quitter"
			)
			)
		{

			Application.Quit();
		}
	}
}

