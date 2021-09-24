using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using TMPro;

public class StatHolder : MonoBehaviour
{
    public float stored_value;
    public GameObject text_display;
    public GameObject text_display_prefab;

    // Start is called before the first frame update
    public void UpdateValue(float value)
    {
        if(text_display == null)
        {
            MakeTextDisplay(Vector3.zero);
        }
        stored_value = value;
        text_display.GetComponent<TextMeshPro>().SetText(value.ToString());
    }

    public void UpdateValue(string value)
    {
        if (text_display == null)
        {
            MakeTextDisplay(Vector3.zero);
        }
        text_display.GetComponent<TextMeshPro>().SetText(value);
    }

    public void MakeTextDisplay(Vector3 relative_location)
    {
        text_display = Instantiate(text_display_prefab, transform.position + relative_location, Quaternion.identity);
    }

    public void MakeTextDisplay()
    {
        text_display = Instantiate(text_display_prefab, transform.position, Quaternion.identity);
    }
}
