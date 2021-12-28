using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System;
using Mirror;

public class SnapIntoPlace : NetworkBehaviour
{

    [Client]
    void Update()
    {
        if (isLocalPlayer)
        {
            CmdSnap();
        }
    }

    [Command]
    void CmdSnap()
    {
        RpcSnap();
    }
    [ClientRpc]
    private void RpcSnap() => transform.position = new Vector2((float)Math.Round(transform.position.x), (float)Math.Round(transform.position.y));
}
