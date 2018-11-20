using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Tetris : MonoBehaviour {
	public int field_row = 20, field_column = 10;
	[Range(0.01f, 1.0f)]public float low_spd_interval = 0.5f;
	[Range(0.01f, 1.0f)]public float high_spd_interval = 0.02f;
	[Range(0.1f, 1.0f)]public float erease_interval = 0.8f;
	public Vector2 field_size = new Vector2(4.5f, 9.0f);
	[SerializeField]GameObject block_prefab;
	[SerializeField]Vector2Int nextRockPos;
	
	GameObject [,]blocks;
	enum FieldType{Empty, Rock};
	GridField field;
	IEnumerator<float> logic;
	float timeLine = 0.0f;
	float fall_interval;

	[SerializeField]RockShape []shapes;
	class Rock {
		public int [,] shape;
		public GameObject []icons;
		public int originX;
		public int originY;
	};
	Rock curRock, nextRock;

	public enum RunState {Stop=0, Run, Pause};
	RunState run_state = RunState.Stop;

	void Start() {
		Debug.Assert(block_prefab);
		Debug.Assert(field_row >= 10 && field_column >= 8);
		Debug.Assert(null != shapes && shapes.Length >= 2);

		if(nextRockPos == new Vector2Int(0, 0))
			nextRockPos = new Vector2Int(field_column+2, field_row-4);

		fall_interval = low_spd_interval;
	}

	void Update() {
		if(Input.GetKeyDown(KeyCode.Space))
			changeRunState();
		if(RunState.Run != run_state)
			return;
		
		//Todo: process when logic goes to end
		if(!logic.MoveNext())
			run_state = RunState.Stop;
		//Coroutine co = StartCoroutine(gameLogic(field).GetEnumerator());
	}

	public RunState changeRunState() {
		switch(run_state) {
		case RunState.Stop:
			initGame();			
			run_state = RunState.Run;
			break;
		case RunState.Pause:
			run_state = RunState.Run;
			break;
		case RunState.Run:
			run_state = RunState.Pause;
			break;
		}
		return run_state;
	}

	void initGame() {
		field = new GridField(field_row, field_column, field_size, this.transform);

		if(null != blocks) {
			foreach(var b in blocks) {
				if(b) Destroy(b);
			}
		}
		blocks = new GameObject[field_column, field_row];

		if(null != curRock && null != curRock.icons) {
			foreach(var o in curRock.icons)
				if(o) Destroy(o);
		}
		if(null != nextRock && null != nextRock.icons) {
			foreach(var o in nextRock.icons)
				if(o) Destroy(o);
		}
		nextRock = newRock(shapes[Random.Range(0, shapes.GetLength(0))]);
		genRock();

		logic = gameLogic(field).GetEnumerator();
		
		timeLine = 0.0f;
		Random.InitState((int)(Time.time*1000));
	}

	IEnumerable<float> gameLogic(GridField field) {
		int []checkLines;
		IEnumerator<float> e = genRock().GetEnumerator();
		while(true) {
			if(!e.MoveNext()) {
				e = genRock().GetEnumerator();
				Debug.Assert(e.MoveNext());
			}
			while(stepFallRock()) {
				yield return 0.0f;
			}
			if(isTopLineGotten())
				break;
			stickRock();
			if(checkFullLines(out checkLines)) {
				foreach(int step in stepEreaseLines(checkLines))
					yield return 0.0f;
			}
		}
		foreach(var icon in curRock.icons)
			icon.GetComponent<SpriteRenderer>().color = Color.red;
	}

	bool stepFallRock() {
		timeLine += Time.deltaTime;
		if(!procInput())
			return false;
		if(timeLine >= fall_interval) {
			timeLine -= fall_interval;

			int x = curRock.originX;
			int y = curRock.originY - 1;
			if(tryMoveRockTo(curRock, x, y))
				moveRockTo(curRock, x, y);
			else
				return false;
		}
		return true;
	}

	bool procInput() {
		if(Input.GetKeyDown(KeyCode.UpArrow)) {
			deform();
		}

		if(Input.GetKeyDown(KeyCode.LeftArrow)) {
			int x = curRock.originX - 1;
			int y = curRock.originY;
			if(tryMoveRockTo(curRock, x, y))
				moveRockTo(curRock, x, y);
		}
		else if(Input.GetKeyDown(KeyCode.RightArrow)) {
			int x = curRock.originX + 1;
			int y = curRock.originY;
			if(tryMoveRockTo(curRock, x, y))
				moveRockTo(curRock, x, y);
		}

		fall_interval = Input.GetKey(KeyCode.DownArrow) ? high_spd_interval : low_spd_interval;
		return true;
	}

	bool deform() {
		int [,] ns = curRock.shape.Clone() as int[,];
		int t;
		for(int i=0; i!=ns.GetLength(0); ++i) {
			//rotate 90 degrees
			t = ns[i, 0];
			ns[i, 0] = -ns[i, 1];
			ns[i, 1] = t;
		}
		for(int i=0; i!=ns.GetLength(0); ++i) {
			int ny = curRock.originY + ns[i, 1];
			int? v = field.checkValid(curRock.originX + ns[i,0], ny);
			if(v != (int)FieldType.Empty && ny < field_row)
				return false;
		}
		curRock.shape = ns;
		moveRockTo(curRock, curRock.originX, curRock.originY);
		return true;
	}

	bool tryMoveRockTo(Rock r, int x, int y) {
		var s = curRock.shape;
		for(int i=0; i!=s.GetLength(0); ++i) {
			int nx = x + s[i,0];
			int ny = y + s[i,1];
			int? t = field.checkValid(nx, ny);
			if(t != (int)FieldType.Empty && ny < field_row)
				return false;
		}
		return true;
	}

	void moveRockTo(Rock r, int x, int y) {
		r.originX = x;
		r.originY = y;

		var shape = curRock.shape;
		for(int i=0; i!=curRock.icons.Length; ++i)
			curRock.icons[i].transform.position = field.calcPos(x + shape[i,0], y + shape[i,1]);
	}

	void stickRock() {
		var s = curRock.shape;
		for(int i=0; i!=s.GetLength(0); ++i) {
			int x = curRock.originX + s[i,0];
			int y = curRock.originY + s[i,1];
			blocks[x, y] = curRock.icons[i];
			field.setGrid(x, y, (int)FieldType.Rock);
		}
	}

	bool isTopLineGotten() {
		for(int i=0; i<curRock.shape.GetLength(0); ++i) {
			if(curRock.originY + curRock.shape[i, 1] >= field_row - 1)
				return true;
		}
		return false;
	}

	//last element of checkLines is the index of lowest empty line
	bool checkFullLines(out int []checkLines) {
		int []l = new int[field_row];
		int count = 0;
		int empty_top = field_row - 1;
		for(int y=0; y < field_row; ++y) {
			//can be optimized with current rock
			if(true == field.checkRow(y, (int)FieldType.Rock))
				l[count++] = y;
			else if(true == field.checkRow(y, (int)FieldType.Empty)) {
				empty_top = y;
				break;
			}
		}
		if(0 == count) {
			checkLines = null;
			return false;
		}
		checkLines = new int[count + 1];
		for(int i=0; i<count; ++i)
			checkLines[i] = l[i];
		checkLines[count] = empty_top;
		return true;
	}

	IEnumerable<float> stepEreaseLines(int []splits) {
		foreach(int l in splits)
			Debug.Assert(l >= 0 && l < field_row);
		float ei_r = 1.0f / erease_interval;
		float time = 0.0f;
		int cur = 0;
		while(time <= erease_interval) {
			time += Time.deltaTime;
			int to = (int)((field_column) * (time * ei_r));
			if(to > field_column)
				to = field_column;
			for(int x=cur; x<to; ++x) {
				for(int i=0; i<splits.Length-1; ++i) {
					Destroy(blocks[x, splits[i]]);
					blocks[x, splits[i]] = null;
				}
			}
			cur = to;
			yield return 0.0f;
		}
		
		int sli = 1; //splite line indices
		cur = splits[0];
		int line = cur + 1;
		while(sli < splits.Length) {
			while(line < splits[sli]) {
				moveLine(line++, cur++);
			}
			++sli;
			++line;
		}
	}

	bool moveLine(int from, int to) {
		if(field.moveRow(from, to, (int)FieldType.Empty)) {
			for(int x=0; x<field_column; ++x) {
				blocks[x, to] = blocks[x, from];
				if(blocks[x, to])
					blocks[x, to].transform.position = field.calcPos(x, to);
				blocks[x, from] = null;
			}
			return true;
		}
		else
			return false;
	}

	IEnumerable<float> genRock() {
		RockShape []ss = new RockShape[shapes.Length];
		for(int s=0; s!=shapes.Length; ++s) {
			int i = Random.Range(0, shapes.Length);
			ss[s] = shapes[i];
		}

		foreach(RockShape s in ss) {
			curRock = nextRock;
			moveRockTo(curRock, field_column/2, field_row-1);
			foreach(var icon in curRock.icons)
				icon.GetComponent<SpriteRenderer>().color = Color.green;

			nextRock = newRock(s);
			yield return 0.0f;
		}
	}

	Rock newRock(RockShape s) {
		Rock r = new Rock();
		r.shape = new int[s.offset.Count,2];
		r.icons = new GameObject[s.offset.Count];
		for(int i=0; i<s.offset.Count; ++i) {
			r.shape[i, 0] = s.offset[i].x - s.origin.x;
			r.shape[i, 1] = s.offset[i].y - s.origin.y;
			r.icons[i] = Instantiate(block_prefab,
				field.calcPos(nextRockPos.x + r.shape[i,0], nextRockPos.y + r.shape[i,1]),
				Quaternion.identity,
				this.transform);
			r.icons[i].GetComponent<SpriteRenderer>().color = Color.yellow;
		}
		r.originX = -1;
		r.originY = -1;
		return r;
	}
}