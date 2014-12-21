using UnityEngine;
using System.Collections;

public class Moveennemy : MonoBehaviour {

	public Vector3 speed = new Vector3(1000, 1000,0);
	

	public Vector3 direction = new Vector3(0, 3,0);
	
	private Vector3 movement;


	void Update()
	{

		movement = new Vector3(
			(speed.x)* direction.x,
			(speed.y) * direction.y,
			(speed.z) * direction.z);
	}

	void FixedUpdate()
	{

			rigidbody.velocity = movement;

	}

}
