#pragma strict

function Start () {

}

function Update () {
	
}

var levelToLoad:String="scenelvl1";
function OnTriggerEnter( other: Collider)
{
	Application.LoadLevel(levelToLoad);

}