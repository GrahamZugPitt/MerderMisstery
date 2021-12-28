using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class DestroyOnClick: MonoBehaviour
{
    public void OnMouseDown()
    {
        Destroy(gameObject);
    }
}
