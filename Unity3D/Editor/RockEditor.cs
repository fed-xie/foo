using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEditor;
using UnityEditorInternal;

[CustomEditor(typeof(RockShape))]
public class TetrisRockEditor : Editor {
    Vector2Int shape_size;
    bool[,] temp_shape;
    Vector2Int origin;

    public void OnEnable() {
        var shape = (RockShape)target;
        if(null != shape.offset) {
            int xmax=0, ymax=0;
            List<Vector2Int> o = shape.offset;
            for(int i=0; i<o.Count; ++i) {
                if(o[i].x>xmax)
                    xmax = o[i].x;
                if(o[i].y>ymax)
                    ymax = o[i].y;
            }
            shape_size = new Vector2Int(xmax+1, ymax+1);
            temp_shape = new bool[xmax+1, ymax+1];

            for(int i=0; i<o.Count; ++i)
                temp_shape[o[i].x, o[i].y] = true;
        }
        origin = shape.origin;
    }

    public override void OnInspectorGUI() {
        var shape = (RockShape)target;

        shape_size = EditorGUILayout.Vector2IntField("Shape size:", shape_size);

        EditorGUILayout.LabelField("Rock shape:");
        if(null == temp_shape || shape_size.x != temp_shape.GetLength(0) || shape_size.y != temp_shape.GetLength(1))
            temp_shape = new bool[shape_size.x, shape_size.y];
        
        for(int y=0; y<shape_size.y; ++y) {
            EditorGUILayout.BeginHorizontal();
            for(int x=0; x<shape_size.x; ++x) {
                GUI.backgroundColor = temp_shape[x,y] ? Color.green : Color.gray;
                if(GUILayout.Button("", 
                    GUILayout.Width(EditorGUIUtility.singleLineHeight), 
                    GUILayout.Height(EditorGUIUtility.singleLineHeight))) {
                    temp_shape[x,y] = !temp_shape[x,y];
                }
            }
            EditorGUILayout.EndHorizontal();
        }

        EditorGUILayout.LabelField("Origin point:");
        for(int y=0; y<shape_size.y; ++y) {
            EditorGUILayout.BeginHorizontal();
            for(int x=0; x<shape_size.x; ++x) {
                GUI.backgroundColor = (origin.x==x && origin.y==y) ? Color.green : Color.gray;
                if(GUILayout.Button("", 
                    GUILayout.Width(EditorGUIUtility.singleLineHeight), 
                    GUILayout.Height(EditorGUIUtility.singleLineHeight)))
                    origin = new Vector2Int(x,y);
            }
            EditorGUILayout.EndHorizontal();
        }

        GUI.backgroundColor = Color.white;
        if(GUILayout.Button("Save")) {
            shape.offset = new List<Vector2Int>();

            for(int y=0; y<shape_size.y; ++y) {
                for(int x=0; x<shape_size.x; ++x) {
                    if(temp_shape[x,y])
                        shape.offset.Add(new Vector2Int(x,y));
                }
            }

            shape.origin = origin;

            EditorUtility.SetDirty(target);
            AssetDatabase.SaveAssets();
        }
    }
}