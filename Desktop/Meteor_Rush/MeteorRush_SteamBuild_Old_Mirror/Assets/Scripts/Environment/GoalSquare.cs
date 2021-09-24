using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class GoalSquare : MonoBehaviour
{
    public int square_color;
    public Sprite red_square;
    public Sprite blue_square;

    enum Team
    {
        RED,
        BLUE,
        NONE
    }

    public void color(int given_color)
    {
        square_color = given_color;
        GetComponent<SpriteRenderer>().sprite = given_color == (int)Team.RED ? red_square : blue_square;
    }
}
