using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

[RequireComponent (typeof(Button))]
public class StartButton : MonoBehaviour {

	[SerializeField] GluttonousSnake gsnake;
	// Use this for initialization
	void Start () {
		Debug.Assert(gsnake);
		this.GetComponent<Button>().onClick.AddListener(click);
	}
	
	void click() {
		var state = gsnake.changeRunState();
		var text = this.transform.Find("Text").GetComponent<Text>().text;
		switch(state)
		{
		case GluttonousSnake.RunState.Run:
			this.transform.Find("Text").GetComponent<Text>().text = "Pause";
			break;
		case GluttonousSnake.RunState.Pause:
			this.transform.Find("Text").GetComponent<Text>().text = "Run";
			break;
		case GluttonousSnake.RunState.Stop:
			this.transform.Find("Text").GetComponent<Text>().text = "Start";
			break;
		}
	}
}
