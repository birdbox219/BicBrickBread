using System;
using System.Runtime.InteropServices;
using UnityEngine;

public class BoardGamePlugin : MonoBehaviour
{
    private const string DLL_NAME = "UnityWrapper";

    [DllImport(DLL_NAME, CallingConvention = CallingConvention.Cdecl)]
    public static extern int InitGame(int gameId);

    [DllImport(DLL_NAME, CallingConvention = CallingConvention.Cdecl)]
    public static extern void CleanupGame();

    [DllImport(DLL_NAME, CallingConvention = CallingConvention.Cdecl)]
    public static extern void ResetGame(int gameId);

    [DllImport(DLL_NAME, CallingConvention = CallingConvention.Cdecl)]
    public static extern void GetBoardDimensions(out int rows, out int cols);

    [DllImport(DLL_NAME, CallingConvention = CallingConvention.Cdecl)]
    public static extern int PerformMove(int x, int y, int playerVal, int extraChar);

    [DllImport(DLL_NAME, CallingConvention = CallingConvention.Cdecl)]
    public static extern IntPtr GetBoardState();

    [DllImport(DLL_NAME, CallingConvention = CallingConvention.Cdecl)]
    public static extern bool CheckWin(int playerVal);

    [DllImport(DLL_NAME, CallingConvention = CallingConvention.Cdecl)]
    public static extern bool CheckDraw();

    [DllImport(DLL_NAME, CallingConvention = CallingConvention.Cdecl)]
    public static extern bool IsGameOver();

    [DllImport(DLL_NAME, CallingConvention = CallingConvention.Cdecl)]
    public static extern int GetAiMove(int playerVal);

    public int CurrentGameId = 1; // Set in Inspector

    void Start()
    {
        StartGame(CurrentGameId);
    }

    public void StartGame(int id)
    {
        Debug.Log($"Initializing Game ID: {id}...");
        int result = InitGame(id);
        if (result == 0)
        {
            Debug.Log("Game Initialized Successfully.");
            UpdateBoard();
        }
        else
        {
            Debug.LogError($"Failed to init game. Error code: {result}");
        }
    }

    void OnDestroy()
    {
        CleanupGame();
    }

    public void PlayerMove(int x, int y, int extraChar = 0)
    {
        int result = PerformMove(x, y, 1, extraChar); // Player 1
        if (result == 0)
        {
            Debug.Log("Move successful");
            UpdateBoard();
            
            if (CheckWin(1)) Debug.Log("Player 1 Wins!");
            else if (CheckDraw()) Debug.Log("Draw!");
            else
            {
                // AI Turn
                if (GetAiMove(2) == 0)
                {
                    UpdateBoard();
                    if (CheckWin(2)) Debug.Log("AI Wins!");
                }
            }
        }
        else
        {
            Debug.Log("Invalid Move");
        }
    }

    void UpdateBoard()
    {
        int rows, cols;
        GetBoardDimensions(out rows, out cols);
        
        IntPtr ptr = GetBoardState();
        if (ptr != IntPtr.Zero && rows > 0 && cols > 0)
        {
            int size = rows * cols;
            int[] board = new int[size];
            Marshal.Copy(ptr, board, 0, size);
            
            // Debug print board
            string s = "Board:\n";
            for(int r=0; r<rows; ++r) {
                for(int c=0; c<cols; ++c) {
                    s += board[r*cols + c] + " ";
                }
                s += "\n";
            }
            Debug.Log(s);
        }
    }
}
