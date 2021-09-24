using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using Mirror;
using UnityEngine.Tilemaps;
using UnityEngine.SceneManagement;
using Steamworks;

public class NetworkManagerExample : NetworkManager
{
    public Transform spawn;
    public Transform spawn_two;
    public GameObject boardPrefrab;
    public GameObject red_base;
    public GameObject blue_base;
    public GameObject[] bases = new GameObject[2];

    override public void OnServerAddPlayer(NetworkConnection conn)
    {
        Transform start = numPlayers == 0 ? spawn : spawn_two;
        GameObject player;
        if (numPlayers == 1)
        {
            player = Instantiate(blue_base, start.position, start.rotation);
            bases[1] = player;
            NetworkServer.AddPlayerForConnection(conn, player);
        }
        if (numPlayers == 0)
        {
            player = Instantiate(red_base, start.position, start.rotation);
            bases[0] = player;
            NetworkServer.AddPlayerForConnection(conn, player);
        }
        if (numPlayers == 2)
        {
            GameObject board = Instantiate(boardPrefrab, new Vector3((float)-.5,(float)-.5,0), Quaternion.identity);
            board.transform.SetParent(transform);
            NetworkServer.Spawn(board);
            BoardScript boardScript = board.GetComponent(typeof(BoardScript)) as BoardScript;
            boardScript.initialize(bases);
        }
    }

    public void DoShutdown()
    {
        StopHost();

        //Start();
        //Shutdown();
        //networkSceneName = "";
    }
}
