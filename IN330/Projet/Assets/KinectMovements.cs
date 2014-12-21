using UnityEngine;
using System.Collections;

public class KinectMovements : MonoBehaviour {

	public Vector3 direction = new Vector3(0, 0, 0);
	public GameObject FPS;
	public GameObject Hips;
	public GameObject Shoulder;
	// Use this for initialization
	void Start () {

	}
	
	// Update is called once per frame
	void Update () {
		direction.x = Hips.transform.position.x - Shoulder.transform.position.x;
		direction.z = Hips.transform.position.y - Shoulder.transform.position.y;
		FPS.transform.position += direction * 5;
	}
}
