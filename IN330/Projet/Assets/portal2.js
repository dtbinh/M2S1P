#pragma strict

function Start () {

}

function Update () {
	
}

var levelToLoad:String="scenelvl3";
function OnTriggerEnter( other: Collider)
{
	Application.LoadLevel(levelToLoad);

}