using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CameraFollow : MonoBehaviour {
	[SerializeField] public GameObject target;
	[SerializeField] public Vector3 offset;
	[SerializeField] public float smoothTime = 0.4f;
	private Vector3 m_velocity;
	
	// Use this for initialization
	void Start () {
		if(null == target)
			target = GameObject.FindWithTag("Player");
		Debug.Assert(null != target);
		
		if(Vector3.zero == offset)
			offset = this.transform.position - target.transform.position;
	}

	void changeTarget(GameObject obj, Vector3 off)
	{
		target = obj;
		offset = off;
	}

	public void changeTarget(GameObject obj)
	{
		changeTarget(obj, offset);
	}

	// Update is called once per frame
	void Update () {
		var tarpos = target.transform.position + offset;
		transform.position = Vector3.SmoothDamp(transform.position, tarpos, ref m_velocity, smoothTime);
	}
}
