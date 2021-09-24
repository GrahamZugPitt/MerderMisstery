using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using TMPro;

public class SetEndScreen : MonoBehaviour
{

    void Start()
    {
        int red_score = PlayerPrefs.GetInt("red_score");
        int blue_score = PlayerPrefs.GetInt("blue_score");
        int my_color = PlayerPrefs.GetInt("my_color");

        if(my_color == 0)
        {
            if(red_score > blue_score)
            {
                GetComponent<TextMeshProUGUI>().SetText("YOU WON");
                return;
            }
            if (red_score < blue_score)
            {
                GetComponent<TextMeshProUGUI>().SetText("YOU LOST");
                return;
            }
        }
        if (my_color == 1)
        {
            if (red_score > blue_score)
            {
                GetComponent<TextMeshProUGUI>().SetText("YOU LOST");
                return;
            }
            if (red_score < blue_score)
            {
                GetComponent<TextMeshProUGUI>().SetText("YOU WON");
                return;
            }
        }
        GetComponent<TextMeshProUGUI>().SetText("DRAW");
    }
}
