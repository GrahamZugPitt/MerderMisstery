using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using Mirror;
using UnityEditor;
using UnityEngine.Tilemaps;

public class Punching_Bag : ShipScript
{
    public int punching_bag_kicking_distance = 5;
    override public void showRange(int dist, GameObject square, GameObject enemy)
    {
        if (isBeingKicked)
        {
            dist = punching_bag_kicking_distance;
        }
        KillSquares();
        Tilemap tilemap = GetComponentInParent<Transform>().GetComponentInParent<Tilemap>();
        Vector3[] takenPositions = GetComponentInParent<Transform>().GetComponentInParent<BoardScript>().shipPositions();
        squares = new GameObject[(dist * 2 + 1) * (dist * 2 + 1)];
        locations = new Vector3[(dist * 2 + 1) * (dist * 2 + 1)];
        showLegalMoves(dist, square, enemy);
    }
}
