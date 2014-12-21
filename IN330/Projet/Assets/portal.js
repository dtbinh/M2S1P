#pragma strict

function Start () {

}

function Update () {
	
}

var levelToLoad:String="chargementscene2";
function OnTriggerEnter( other: Collider)
{
	Application.LoadLevel(levelToLoad);

}