using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using Mirror;

public class PlanetScript : NetworkBehaviour
{
    public int planet_metal = 0;
    public int avaliable_metal = 0;
    public int team = 0;
    enum Team
    {
        NONE,
        RED,
        BLUE
    }
}
