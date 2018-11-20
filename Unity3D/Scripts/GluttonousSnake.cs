using System.Collections;
using System.Collections.Generic;
using UnityEngine;

//Gluttonous Snake
public class GluttonousSnake : MonoBehaviour {
	[SerializeField]GameObject block_prefab;
	[SerializeField]int field_row = 10, field_column = 10;
	[Range(0.1f, 1.0f)][SerializeField]float time_interval = 0.5f;
	[SerializeField]Vector2 field_size = new Vector2(9.0f, 9.0f);
	[SerializeField]Color head_color = Color.white;

	public enum RunState {Stop, Run, Pause};
	RunState run_state = RunState.Stop;
	GridField field;

	enum GameState {Begin, Loop, Win, Fail, End};
	GameState game_state = GameState.Begin;
	enum FieldType {Empty = 0, Food, Snake};
	struct SnakeStruct {
		public GameObject []icons;
		public int[]posX;
		public int[]posY;
		public int head, tail;
		public enum Orientaion {Up = 0, Left, Down, Right};
		public Orientaion ori;
	};
	SnakeStruct snake;
	struct FoodStruct {
		public GameObject icon;
		public int x,y;
	};
	FoodStruct food;
	float timeline = 0.0f;

	// Use this for initialization
	void Start () {
		Debug.Assert(block_prefab);
		Debug.Assert(field_row > 4 && field_column > 4);
		Debug.Assert((field_column & 1) == 0 || (field_row & 1) == 0);
	}
	
	// Update is called once per frame
	void Update () {
		if(Input.GetKeyDown(KeyCode.Space))
			changeRunState();
		if(RunState.Run != run_state)
			return;

		gameLogic();
	}

	void gameLogic() {
		switch(game_state) {
		case GameState.Begin:
			initGame();
			game_state = GameState.Loop;
			break;
		case GameState.Loop:
			processInput();
			game_state = ClimbSnake();
			break;
		case GameState.Win:
		case GameState.Fail:
			var c = game_state == GameState.Win ? Color.cyan : Color.yellow;
			foreach(var icon in snake.icons) {
				if(icon)
					icon.GetComponent<SpriteRenderer>().color = c;
			}
			game_state = GameState.End;
			break;
		case GameState.End:
			run_state = RunState.Stop;
			game_state = GameState.Begin;
			break;
		default:
			Debug.Assert("Undefined game logic!" == null);
			break;
		}
	}

	void initGame() {
		field = new GridField(field_row, field_column, field_size, this.transform);
		field.clear((int)FieldType.Empty);
		if(null != snake.icons) {
			foreach(var icon in snake.icons) {
				if(icon)
					Destroy(icon);
			}
		}

		snake.head = snake.tail = 0;
		int oriX = field_column/2;
		int oriY = field_row/2;
		int maxlen = field_column * field_row;
		snake.icons = new GameObject[maxlen];
		snake.icons[snake.head] = Instantiate(block_prefab, field.calcPos(oriX, oriY), Quaternion.identity, this.transform);
		snake.icons[snake.head].GetComponent<SpriteRenderer>().color = head_color;
		snake.posX = new int[maxlen];
		snake.posY = new int[maxlen];
		snake.posX[snake.head] = oriX;
		snake.posY[snake.head] = oriY;
		snake.ori = SnakeStruct.Orientaion.Down;
		field[oriX, oriY] = (int)FieldType.Snake;

		food.x = 2;
		food.y = 2;
		if(null == food.icon) {
			food.icon = Instantiate(block_prefab, field.calcPos(food.x,food.y), Quaternion.identity, this.transform);
			food.icon.GetComponent<SpriteRenderer>().color = Color.red;
		}
		else
			food.icon.transform.position = field.calcPos(food.x, food.y);
		
		field[food.x, food.y] = (int)FieldType.Food;

		Random.InitState((int)(Time.time*1000.0f));
	}

	void processInput() {
		SnakeStruct.Orientaion no = snake.ori;
		if(Input.GetKeyDown(KeyCode.UpArrow))
			no = SnakeStruct.Orientaion.Up;
		else if(Input.GetKeyDown(KeyCode.DownArrow))
			no = SnakeStruct.Orientaion.Down;
		else if(Input.GetKeyDown(KeyCode.LeftArrow))
			no = SnakeStruct.Orientaion.Left;
		else if(Input.GetKeyDown(KeyCode.RightArrow))
			no = SnakeStruct.Orientaion.Right;
		
		if((int)no != ((int)snake.ori + 2) % 4)//No opposite direction
			snake.ori = no;
	}

	GameState ClimbSnake() {
		timeline += Time.deltaTime;
		if(timeline > time_interval) {
			timeline -= time_interval;

			int nx=0, ny=0;
			switch(snake.ori) {
			case SnakeStruct.Orientaion.Up:		ny = 1; break;
			case SnakeStruct.Orientaion.Down:	ny = -1; break;
			case SnakeStruct.Orientaion.Left:	nx = -1; break;
			case SnakeStruct.Orientaion.Right:	nx = 1; break;
			}

			nx += snake.posX[snake.head];
			ny += snake.posY[snake.head];

			int? v = field.checkValid(nx, ny);
			//Debug.LogFormat("npos = {{{0}, {1}}}", nx, ny);
			if((int)FieldType.Snake == v || null == v)
				return GameState.Fail;
			else if((int)FieldType.Empty == v) {
				int ot = snake.tail;
				int nh = (snake.head + 1) % snake.icons.Length;

				snake.icons[snake.head].GetComponent<SpriteRenderer>().color = Color.white;
				snake.icons[nh] = snake.icons[ot];
				snake.icons[ot] = null;
				snake.icons[nh].transform.position = field.calcPos(nx, ny);
				snake.icons[nh].GetComponent<SpriteRenderer>().color = head_color;

				snake.head = nh;
				snake.tail = (snake.tail + 1) % snake.icons.Length;

				snake.posX[nh] = nx;
				snake.posY[nh] = ny;
				field.moveGrid(snake.posX[ot], snake.posY[ot], nx, ny, (int)FieldType.Empty);
			}
			else if((int)FieldType.Food == v) {
				int maxlen = snake.icons.Length;
				int nh = (snake.head + 1) % maxlen;

				snake.icons[snake.head].GetComponent<SpriteRenderer>().color = Color.white;
				snake.icons[nh] = Instantiate(block_prefab, field.calcPos(nx, ny), Quaternion.identity, this.transform);
				snake.icons[nh].GetComponent<SpriteRenderer>().color = head_color;
				snake.posX[nh] = nx;
				snake.posY[nh] = ny;
				snake.head = nh;
				field[nx, ny] = (int)FieldType.Snake;

				if(snake.tail == (nh + 1 ) % maxlen)
					return GameState.Win;
				int snakelen = (nh + maxlen - snake.tail + 1) % maxlen;
				int availspace = snake.icons.Length - snakelen;
				moveFood(availspace);
			}
		}
		return GameState.Loop;
	}

	void moveFood(int availspace) {
		int rand = Random.Range(0, availspace);
		int count = 0;
		for(int y=0; y<field_row; ++y) {
			for(int x=0; x<field_column; ++x) {
				if((int)FieldType.Empty == field[x,y]) {
					if(count == rand) {
						food.x = x;
						food.y = y;
						food.icon.transform.position = field.calcPos(x,y);
						field[x, y] = (int)FieldType.Food;
						return;
					}
					++count;
				}
			}
		}
	}

	public RunState changeRunState()
	{
		switch(run_state)
		{
		case RunState.Stop:
			//initField();
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
}
