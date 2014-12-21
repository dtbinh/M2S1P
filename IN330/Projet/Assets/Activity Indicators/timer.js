#pragma strict

function Start () {

}

//A simple countdown timer
var myTimer : float = 5.0;
var levelToLoad:String = "scenelvl0";
 
function Update () {
 if(myTimer > 0){
  myTimer -= Time.deltaTime;
 }
 if(myTimer <= 0){
  Application.LoadLevel(levelToLoad);
 }
}