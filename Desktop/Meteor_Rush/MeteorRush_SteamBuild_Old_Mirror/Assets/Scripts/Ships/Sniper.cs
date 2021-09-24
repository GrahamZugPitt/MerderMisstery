using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System;
using Mirror;
using UnityEngine.Tilemaps;
using System.Linq;


public class Sniper : ShipScript


{
    /*override public void Kick()
    {
        ShipScript ship = GetComponentInParent<BoardScript>().GetShipByPosition(dest);
        if (!ship.GetComponent<ShipScript>().hasAuthority)
        {
            SniperKick();
            return;
        }
        for (int j = 0; j < length; j++)
            if (squares[j] != null)
            {
                GetComponentInParent<BaseScript>().enemy_squares_displayed[(int)squares[j].transform.position.x, (int)squares[j].transform.position.y] = false;
                Destroy(squares[j]);
            }
        ship.kickedByFriendly = true;
        ship.isBeingKicked = true;
        ship.showRange(kickingDistance, movementSquare, enemySquare);
        ship.isMoving = true;
        ship.origin = GetComponentInParent<BoardScript>().GetShipByPosition(dest).GetPosition();
        hasChosenKickingTarget = true;
    }
    public void SniperKick()
    {
        ShipScript ship = GetComponentInParent<BoardScript>().GetShipByPosition(dest);
        for (int j = 0; j < length; j++)
            if (squares[j] != null)
                Destroy(squares[j]);
        if (ship.hasAuthority)
        {
            ship.kickedByFriendly = true;
        }
        ship.CmdDestroy();
        CmdMove(dest);
    }

    override public void showRange(int dist, GameObject square, GameObject enemy)
    {
        Tilemap tilemap = GetComponentInParent<Transform>().GetComponentInParent<Tilemap>();
        Vector3[] takenPositions = GetComponentInParent<Transform>().GetComponentInParent<BoardScript>().shipPositions();
        squares = new GameObject[(dist * 2 + 1) * (dist * 2 + 1)];
        locations = new Vector3[(dist * 2 + 1) * (dist * 2 + 1)];
        if (isBeingKicked)
        {
            showLegalMoves(dist, square, enemy);
            return;
        }
        showLegalMovesSniper(dist, square, enemy);
    }

    public void showLegalMovesSniper(int dist, GameObject square, GameObject enemy)
    {
        squares = new GameObject[(dist * 2 + 1) * (dist * 2 + 1)];
        locations = new Vector3[(dist * 2 + 1) * (dist * 2 + 1)];
        shipsInRange = new Vector3[(dist * 2 + 1) * (dist * 2 + 1)];
        Vector3[] units = { Vector3.right, Vector3.left };


        for (int k = 0; k < locations.Length; k++)
        {
            locations[k] = new Vector3(-1000, -1000, -1000);
            shipsInRange[k] = new Vector3(-1000, -1000, -1000);
        }
        int i = 0;
        int m = 0;
        int b = 0;
        bool first = true;
        bool any = false;
        do
        {
            if (first)
            {
                for (int a = 0; a < units.Length; a++)
                {
                    if (freeSpace(transform.position + units[a]))
                    {
                        locations[i] = transform.position + units[a];
                        squares[i] = Instantiate(square, locations[i], Quaternion.identity);
                        squares[i].GetComponent<movementSquare>().dist = 1;
                        i++;
                        any = true;
                    }
                }
                first = false;
            }

            for (int j = 0; j < units.Length; j++)
            {

                if (any && freeSpace(locations[m] + units[j]) && units[j].Equals(Vector3.right) && locations[m].y == transform.position.y && locations[m].x > transform.position.x) //&& free(locations[m] + units[j]))
                {
                    locations[i] = locations[m] + units[j];
                    squares[i] = Instantiate(square, locations[i], Quaternion.identity);
                    squares[i].GetComponent<movementSquare>().dist = squares[m].GetComponent<movementSquare>().dist + 1;
                    i++;
                }

                if (any && freeSpace(locations[m] + units[j]) && units[j].Equals(Vector3.left) && locations[m].y == transform.position.y && locations[m].x < transform.position.x) //&& free(locations[m] + units[j]))
                {
                    locations[i] = locations[m] + units[j];
                    squares[i] = Instantiate(square, locations[i], Quaternion.identity);
                    squares[i].GetComponent<movementSquare>().dist = squares[m].GetComponent<movementSquare>().dist + 1;
                    i++;
                }

                if (any && !shipsInRange.Contains(locations[m] + units[j]) && spaceTakenByShip(locations[m] + units[j]) && GetComponentInParent<BoardScript>().GetShipByPosition(locations[m] + units[j]).isKickable && !isBeingKicked && !isEnemyBase(locations[m] + units[j]) && !units[j].Equals(Vector3.up) && !units[j].Equals(Vector3.down)) //&& free(locations[m] + units[j]))
                {
                    shipsInRange[b] = locations[m] + units[j];
                    b++;
                }
            }
            m++;
        } while (any && !locations[m].Equals(new Vector3(-1000, -1000, -1000)));

        for (int k = 0; k < b; k++)
        {
            if (((Vector3.Distance(shipsInRange[k], transform.position) <= movement) || shipsInRange[k].y == transform.position.y))
            {
                locations[i] = shipsInRange[k];
                squares[i] = Instantiate(enemy, locations[i], Quaternion.identity);
                i++;
            }
        }

        for (int c = 0; c < units.Length; c++)
        {
            if (
                !locations.Contains(transform.position + units[c])
                && spaceTakenByShip(transform.position + units[c])
                && GetComponentInParent<BoardScript>().GetShipByPosition(transform.position + units[c]).isKickable
                && !isBeingKicked)
            {
                locations[i] = transform.position + units[c];
                squares[i] = Instantiate(enemy, locations[i], Quaternion.identity);
                shipsInRange[b++] = locations[i++];
            }
        }
        for (int j = 0; j < i; j++)
            squares[j].GetComponent<movementSquare>().ship = transform.GetComponent<ShipScript>();
        for (int j = 0; j < i; j++)
        {
            squares[j].GetComponent<movementSquare>().ship = transform.GetComponent<ShipScript>();
            if (GetComponentInParent<BaseScript>().enemy_squares_displayed[(int)squares[j].transform.position.x, (int)squares[j].transform.position.y] && isShowingRangeToEnemy)
                Destroy(squares[j]);
            GetComponentInParent<BaseScript>().enemy_squares_displayed[(int)squares[j].transform.position.x, (int)squares[j].transform.position.y] = true;
        }
        length = i;
    }*/
}
