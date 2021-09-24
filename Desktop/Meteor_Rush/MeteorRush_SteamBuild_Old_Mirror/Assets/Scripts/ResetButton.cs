using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ResetButton : MonoBehaviour
{
    public Sprite resetOff;
    public Sprite resetOn;

    public void OnMouseOver()
    {
        transform.GetComponent<SpriteRenderer>().sprite = resetOn;
    }

    public void OnMouseExit()
    {
        transform.GetComponent<SpriteRenderer>().sprite = resetOff;
    }

    public void OnMouseDown()
    {
        GetComponentInParent<BoardScript>().CmdReset();
    }
}
