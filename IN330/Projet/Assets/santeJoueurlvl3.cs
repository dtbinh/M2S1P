using UnityEngine;
using System.Collections;

public class santeJoueurlvl3 : MonoBehaviour {



	public int SanteMax = 100; 
	public int SanteActu = 100; 
	public float LargeurBarreSante;
	public float LongueurBarreSante;
	public int BouclierMax = 100; 
	public int BouclierActu =0; 
	public float Largeurbouclier;
	public float Longueurbouclier;
	public float hauteurbouclier;

	void Start () { 
		LargeurBarreSante = Screen.width / 2;
		Largeurbouclier = Screen.width / 2;


	} 
	/*perte de vie du player */

	void OnTriggerEnter( Collider collider ) {

		if (collider.tag =="Enemy") {
						collider.enabled = true;
			if(BouclierActu==0)
			{
				SanteActu -= 50;

				if (SanteActu == 0) {
					
					
					Application.LoadLevel ("MenuPerdulvl3");
					
				}
			}
			else
			{
				BouclierActu -=100;
			}
					
		}
		if (collider.tag =="Alliee") {

			BouclierActu += 100;



		}
		
		
	}
	
	void Update () { 
		AjusterSanteActuelle(0); 
	}
	
	void OnGUI () { 
		GUI.Box(new Rect(20, 20, LongueurBarreSante, 30),"Santé :"+SanteActu + "/" + SanteMax);
		GUI.Box(new Rect(20, 60, Longueurbouclier, 30),"Bouclier :"+BouclierActu + "/" + BouclierMax);
	} 
	public void AjusterSanteActuelle(int adj) { 
		SanteActu += adj; 
		BouclierActu += adj;
		
		if (SanteActu < 0) {

			SanteActu = 0;

		}
		
		/*if (SanteActu > SanteMax) 
			SanteActu = SanteMax; */
		
		if (SanteMax < 1) 
			SanteMax = 1; 
		
		LongueurBarreSante = (Screen.width / 4) * (SanteActu / (float)SanteMax); 
		Longueurbouclier = (Screen.width / 6) * (BouclierActu / (float)BouclierMax); 
	}
}
