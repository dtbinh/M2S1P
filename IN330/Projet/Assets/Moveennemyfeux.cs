using UnityEngine;
using System.Collections;

public class Moveennemyfeux : MonoBehaviour {

	public Vector2 speed = new Vector2(1000, 1000);
	

	public Vector2 direction = new Vector2(0, 3);
	
	private Vector2 movement;
	
	void Update()
	{

		movement = new Vector2(
			(speed.x)* direction.x,
			(speed.y) * direction.y);
	}
	
	void FixedUpdate()
	{

		rigidbody.velocity = -movement;
	}

}
