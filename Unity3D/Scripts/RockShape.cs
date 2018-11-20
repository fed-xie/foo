using System.Collections;
using System.Collections.Generic;
using UnityEngine;

[CreateAssetMenu(fileName="NewRockShape", menuName="Tetris/NewRockShape")]
public class RockShape : ScriptableObject {
	public List<Vector2Int> offset;
	public Vector2Int origin;
};