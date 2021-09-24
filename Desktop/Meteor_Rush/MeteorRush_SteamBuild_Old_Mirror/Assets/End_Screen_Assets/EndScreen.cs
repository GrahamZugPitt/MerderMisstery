using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using Steamworks;
using UnityEngine.SceneManagement;

public class EndScreen : MonoBehaviour
{
    // Start is called before the first frame update
    public void GoBack()
    {
        SceneManager.LoadScene("Title_Screen");
    }

    public void QuitGame()
    {
        Application.Quit();
    }
}
