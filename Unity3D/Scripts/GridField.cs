using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class GridField {
	protected int [,]grid_type;//[y, x]
	protected int row, column;
	protected float interval_x, interval_y;
	protected Vector3 offset;
	protected Transform parent;

	public GridField(int row, int column, Vector2 size, Transform parent) {
		rebuild(row, column, size, parent);
	}

	void rebuild(int row, int column, Vector2 size, Transform parent) {
		grid_type = new int[row, column];
		this.row = row;
		this.column = column;
		this.parent = parent;
		interval_x = size.x / column;
		interval_y = size.y / row;
		offset = new Vector3(
			(-size.x + interval_x) * 0.5f,
			(-size.y + interval_y) * 0.5f,
			0.0f);
	}

	public void clear(int value = 0) {
		for(int y=0; y<row; ++y) {
			for(int x=0; x<column; ++x) {
				grid_type[y, x] = value;
			}
		}
	}

	public int? checkValid(int x, int y) {
		if(x<0 || x>=column || y<0 || y>=row)
			return null;
		else
			return grid_type[y, x];
	}

	public int this[int x, int y] {
		get { return grid_type[y, x]; }
		set { grid_type[y,x] = value; }
	}

	public Vector3 calcPos(int x, int y) {
		float posx = x * interval_x;
		float posy = y * interval_y;
		return new Vector3(posx, posy, 0.0f) + offset + parent.position;
	}

	public int? setGrid(int x, int y, int value) {
		int? v = null;
		if(x<0 || x>=column || y<0 || y>=row) {
			v = grid_type[y, x];
			grid_type[y, x] = value;
		}
		return v;
	}

	public int moveGrid(int fromX, int fromY, int toX, int toY, int deftype = 0) {
		int? fv = checkValid(fromX, fromY);
		Debug.Assert(null != fv && null != checkValid(toX, toY));

		grid_type[toY, toX] = (int)fv;
		grid_type[fromY, fromX] = deftype;
		return (int)fv;
	}

	public bool? checkRow(int y, int type) {
		if(y < 0 || y >= row)
			return null;
		for(int x=0; x<column; ++x) {
			if(grid_type[y, x] != type)
				return false;
		}
		return true;
	}

	public bool moveRow(int from, int to, int deftype = 0) {
		if(from < 0 || from >= row || to < 0 || to >= row)
			return false;
		for(int x=0; x<column; ++x) {
			grid_type[to, x] = grid_type[from, x];
			grid_type[from, x] = deftype;
		}
		return true;
	}
}