using UnityEngine;
using System.Collections;

public class WinScript : MonoBehaviour
{

	void OnGUI()
	{
		const int buttonWidth = 160;
		const int buttonHeight = 60;
		

		
		if (
			GUI.Button(
			// Centré en x, 2/3 en y
			new Rect(
			Screen.width / 2 - (buttonWidth/3 ),
			(Screen.height / 2) - (buttonHeight / 2),
			buttonWidth,
			buttonHeight
			),
			"Back to menu"
			)
			)
		{
			// Retourne au menu
			Application.LoadLevel("MenuPrincipal");
		}
	}
}

