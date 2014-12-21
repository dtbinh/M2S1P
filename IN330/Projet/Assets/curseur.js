var curseur : Texture2D;

function Start() {
    Screen.showCursor = false; 
}

function OnGUI() {
    var positionSouris : Vector3 = Input.mousePosition;
    var positionCurseur : Rect = Rect(positionSouris.x,Screen.height - positionSouris.y,curseur.width,curseur.height);
    GUI.Label(positionCurseur,curseur);
}