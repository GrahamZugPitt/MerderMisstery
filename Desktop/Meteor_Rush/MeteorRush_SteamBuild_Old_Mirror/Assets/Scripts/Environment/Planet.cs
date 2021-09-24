using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using Mirror;

public class Planet : NetworkBehaviour
{
    enum Team
    {
        RED,
        BLUE,
        NONE
    }

    public int player_number = (int)Team.NONE;
    public int growth_rate = 0;
    public int metal = 0;


    public Sprite planet_default;
    public Sprite planet_red;
    public Sprite planet_blue;
    public Sprite rate_display_background;
    public Sprite metal_display_background;

    public GameObject planet_display_prefab;
    public GameObject planet_metal_display;
    public GameObject planet_growth_rate_display;


    public int GetOwnership()
    {
        return player_number;
    }

    public void GenerateMetal()
    {
        metal = metal + growth_rate;
        planet_metal_display.GetComponent<StatHolder>().UpdateValue(metal);
    }

    public void PayForShip(int cost)
    {
        metal = metal - cost;
        planet_metal_display.GetComponent<StatHolder>().UpdateValue(metal);
    }


    public int Capture(int meteors, int team_num)
    {
        if(player_number == team_num)
        {
            return meteors;
        }

        if(player_number != (int)Team.NONE && player_number != team_num)
        {
            growth_rate = 0;
            player_number = (int)Team.NONE;
            metal = 0;
            GetComponent<SpriteRenderer>().sprite = planet_default;
            Destroy(planet_metal_display);
            Destroy(planet_growth_rate_display);
            return meteors;
        }

        if (meteors != 0)
        {
            growth_rate = meteors;
            player_number = team_num;
            if (team_num == (int)Team.RED)
            {
                GetComponent<SpriteRenderer>().sprite = planet_red;
            }
            if (team_num == (int)Team.BLUE)
            {
                GetComponent<SpriteRenderer>().sprite = planet_blue;
            }
            SetCounters();
        }
        return 0;
    }

    public void SetCounters()
    {
        planet_metal_display = Instantiate(planet_display_prefab, transform.position + new Vector3((float)-.4, (float).4,0), Quaternion.identity);
        planet_metal_display.GetComponent<SpriteRenderer>().sprite = metal_display_background;
        planet_metal_display.GetComponent<StatHolder>().MakeTextDisplay();
        planet_metal_display.GetComponent<StatHolder>().UpdateValue(metal);
        planet_metal_display.GetComponent<StatHolder>().text_display.transform.SetParent(planet_metal_display.transform);
        planet_metal_display.transform.SetParent(transform);
        planet_growth_rate_display = Instantiate(planet_display_prefab, transform.position + new Vector3((float).4, (float).4, 0), Quaternion.identity);
        planet_growth_rate_display.GetComponent<SpriteRenderer>().sprite = rate_display_background;
        planet_growth_rate_display.GetComponent<StatHolder>().MakeTextDisplay();
        planet_growth_rate_display.GetComponent<StatHolder>().UpdateValue(growth_rate);
        planet_growth_rate_display.GetComponent<StatHolder>().text_display.transform.SetParent(planet_growth_rate_display.transform);
        planet_growth_rate_display.transform.SetParent(transform);
    }
}
