using UnityEngine;
using System.Collections;

public class santeJoueur : MonoBehaviour {



	public int SanteMax = 100; 
	public int SanteActu = 100; 
	public float LargeurBarreSante;
	public float LongueurBarreSante;
	
	void Start () { 
		LargeurBarreSante = Screen.width / 2; 
	} 
	/*perte de vie du player */

	void OnTriggerEnter( Collider collider ) {

		if (collider.tag =="Enemy") {
						collider.enabled = true;
						SanteActu -= 100;
						SanteMax = SanteActu;
						if (SanteActu == 0) {
							

				//Application.LoadLevel ("MenuPerdulvl2");

						}
				}
		if (collider.tag =="Alliee") {

			SanteActu += 100;
			SanteMax = SanteActu;


		}
		
		
	}
	
	void Update () { 
		AjusterSanteActuelle(0); 
	}
	
	void OnGUI () { 
		GUI.Box(new Rect(20, 20, LongueurBarreSante, 30), SanteActu + "/" + SanteMax);
	} 
	public void AjusterSanteActuelle(int adj) { 
		SanteActu += adj; 
		
		if (SanteActu < 0) {

			SanteActu = 0;

		}
		
		/*if (SanteActu > SanteMax) 
			SanteActu = SanteMax; */
		
		if (SanteMax < 1) 
			SanteMax = 1; 
		
		LongueurBarreSante = (Screen.width / 2) * (SanteActu / (float)SanteMax); 
	}
}
