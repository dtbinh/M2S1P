#pragma strict

public var ObstacleCube : GameObject;
public var numberOfObjects = 20;
public var minX = -25 ;
public var maxX = 25;
public var minZ = -25 ;
public var maxZ = 25 ;
public var t = 1 ; 
public var y = 10; 
function Start () {
for (var i = 0; i < numberOfObjects; i++) {
yield WaitForSeconds(t);
var randomPos = Vector3(Random.Range(minX, maxX), y, Random.Range(minZ, maxZ));
Instantiate(ObstacleCube, randomPos, Quaternion.identity);
}
}