package cs380.othello;

import java.util.List;

public class JPBOthelloPlayer extends OthelloPlayer {
    private int searchDepth_; //maximum depth of minimax search tree
    private int self_; //the number representing the player we are moving for

    public JPBOthelloPlayer(int searchDepth)
    {
        searchDepth_ = searchDepth;
    }

    public OthelloMove getMove(OthelloState state)
    {
        self_ = state.nextPlayerToMove;

        OthelloMove bestMove = null;
        int bestValue = Integer.MIN_VALUE;
        List<OthelloMove> nextMoves = state.generateMoves();
        for(int i=0; i<nextMoves.size(); ++i)
        {
            OthelloState newState = state.applyMoveCloning(nextMoves.get(i)); 
            int value = minimax(newState, 2); //this loop represents depth 1 so we cal with 2
            if(value > bestValue)
            {
                bestValue = value; 
                bestMove = nextMoves.get(i);
            }
        }

        return bestMove;
    }

    //returns the minimax value for a particular subtree
    private int minimax(OthelloState state, int depth)
    {
        if(depth == searchDepth_ || state.gameOver() )
            return heuristic(state);

        if(state.nextPlayerToMove == self_) //maximizing step
        {
            int bestValue = Integer.MIN_VALUE;
            List<OthelloMove> nextMoves = state.generateMoves();
            for(int i=0; i<nextMoves.size(); ++i)
            {
                OthelloState newState = state.applyMoveCloning(nextMoves.get(i)); 
                int value = minimax(newState, depth+1);
                if(value > bestValue)
                    bestValue = value; 
            }

            return bestValue;
        }
        else //minimizing step
        {
            int bestValue = Integer.MAX_VALUE;
            List<OthelloMove> nextMoves = state.generateMoves();
            for(int i=0; i<nextMoves.size(); ++i)
            {
                OthelloState newState = state.applyMoveCloning(nextMoves.get(i)); 
                int value = minimax(newState, depth+1);
                if(value < bestValue)
                    bestValue = value; 
            }

            return bestValue;
        }
    }

    //use score as heuristic but make sure the sign is positive when we are winning
    private int heuristic(OthelloState state)
    {
        if(self_ == OthelloState.PLAYER1)
            return state.score();
        else
            return -state.score();
    };
}
