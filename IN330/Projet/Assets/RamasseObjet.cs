using UnityEngine;
using System.Collections;

public class RamasseObjet : MonoBehaviour
{
	void Bonus(float bonus) {
		print(bonus);
	}

	void OnTriggerExit (Collider collider) {
		if (collider.gameObject.CompareTag ("Player")) {
			Destroy(gameObject);

		}
	}


		// Use this for initialization
		void Start ()
		{
	
		}
	
		// Update is called once per frame
		void Update ()
		{
	
		}
}

