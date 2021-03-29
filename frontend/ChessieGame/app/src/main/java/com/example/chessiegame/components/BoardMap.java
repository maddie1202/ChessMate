package com.example.chessiegame.components;


import java.util.HashMap;
import java.util.List;
import java.util.Map;

import androidx.annotation.RequiresApi;

public class BoardMap {
    Map<Integer, Integer> imageMap;

    public BoardMap() {
        imageMap = new HashMap<>();
        for (int i = 1; i < 9; i++) {
            imageMap.put(i, 700144); // wpawn drawable
        }

        for (int i = 9; i < 19; i++) {
            imageMap.put(i, 700008); // wrook
        }

        for (int i = 19; i < 29; i++) {
            imageMap.put(i, 700022); // wknight
        }

        for (int i = 29; i < 39; i++) {
            imageMap.put(i, 700156); // wbishop
        }

        for (int i = 39; i < 48; i++) {
            imageMap.put(i, 700078); // wqueen
        }

        imageMap.put(48, 700035); // wking

        for (int i = -1; i > -9; i--) {
            imageMap.put(i, 700083); // bpawn
        }

        for (int i = -9; i > -19; i--) {
            imageMap.put(i, 700115); // brook
        }

        for (int i = -19; i > -29; i--) {
            imageMap.put(i, 700108); // bknight
        }

        for (int i = -29; i > -39; i--) {
            imageMap.put(i, 700150); // bbishop
        }

        for (int i = -39; i > -48; i--) {
            imageMap.put(i, 700116); // bqueen
        }

        imageMap.put(-48, 700080); // bking

    }

}

/*
public class Board extends View {

    public Tile[][] board;
    public ImageView[][] piece_board;
    public Rect rect;
    public static final int cols = 8;
    public static final int rows = 8;
    public int size = 0;
    private int x0 = 0;
    private int y0 = 0;

    public boolean dragging;
    public View dragView;
    public TableLayout table;
    private Context context;

    public Board(Context context) {
        super(context); // initialize a new chessboard
        this.board = new Tile[cols][rows];
        this.piece_board = new Piece[cols][rows];
        this.context = context;
        this.table = new TableLayout(context);
        TableLayout.LayoutParams lp = new TableLayout.LayoutParams();
        lp.height = 360;
        lp.width = 360;
        rect = new Rect();
    }

    /*
    @Override
    public void onDraw(Canvas canvas) {

        canvas.drawColor(Color.argb(100, 151, 182, 167));
        fillBoard(canvas);
    }

    /*
     Populate Chess Board
     - for each row, create a new TableRow
     - fill each TableRow with Tiles (which extend CardView)
     - fill each Tile with the appropriate background color
     - for tiles that initially contain pieces, create the appropriate Piece
     - add that Piece (ImageView) to the Tile (CardView)
    */


/*


    public void fillBoard(Canvas canvas) {
        int width = getWidth();
        int height = getHeight();
        size = width / 8;
        this.y0 = (height - width) / 2;

        for (int i = 0; i < rows; i++) {
            TableRow row = new TableRow(this.context);
            TableRow.LayoutParams lp = new TableRow.LayoutParams();
            lp.height = 45;
            lp.width = 360;
            row.setLayoutParams(lp);

            for (int j = 0; j < cols; j++) {
                int xcoord = size * i;
                int ycoord = y0 + size * j;

                // Tile extends CardView
                board[i][j] = new Tile(this.context, i, j);
                Tile.LayoutParams tLayout = new Tile.LayoutParams(Tile.LayoutParams.WRAP_CONTENT, Tile.LayoutParams.WRAP_CONTENT);
                if ((i + j) % 2 == 0) {
                    board[i][j].setBackgroundColor(Color.WHITE);
                } else {
                    board[i][j].setBackgroundColor(Color.argb(100, 151, 182, 181));
                }

                board[i][j].setSquare(rect);
                rect.left = xcoord;
                rect.top = ycoord;
                rect.right = xcoord + size;
                rect.bottom = ycoord + size;
                board[i][j].draw(canvas);
                board[i][j].setOnDragListener(this);

                // Set up initial layout
                Piece p = null;


                /*
                //Pawn placement
                if (i == 1) {
                    Drawable wpawn = getResources().getDrawable(R.drawable.wpawn);
                    wpawn.setBounds(xcoord, ycoord, xcoord+size, ycoord+size);
                    p = new Piece (getContext(), i, j, "wpawn");
                    p.setImageDrawable(wpawn);
                }
                else if(i == 6) {
                    Drawable bpawn = getResources().getDrawable(R.drawable.bpawn);
                    bpawn.setBounds(xcoord, ycoord, xcoord+size, ycoord+size);
                    p = new Piece (getContext(), i, j, "bpawn");
                    p.setImageDrawable(bpawn);
                }
                //Rook
                else if(j == 7 && i == 7 || j == 0 && i == 7){
                    Drawable brook =  getResources().getDrawable(R.drawable.brook);
                    brook.setBounds(xcoord, ycoord, xcoord+size, ycoord+size);
                    p = new Piece (getContext(), i, j, "brook");
                    p.setImageDrawable(brook);
                }
                else if(j == 7 && i == 0 || j == 0 && i == 0){
                    Drawable wrook =  getResources().getDrawable(R.drawable.wrook);
                    wrook.setBounds(xcoord, ycoord, xcoord+size, ycoord+size);
                    p = new Piece (getContext(), i, j, "wrook");
                    p.setImageDrawable(wrook);
                }
                //Knights
                else if(j == 6 && i == 7 || j == 1 && i == 7){
                    Drawable bknight =  getResources().getDrawable(R.drawable.bknight);
                    bknight.setBounds(xcoord, ycoord, xcoord+size, ycoord+size);
                    p = new Piece (getContext(), i, j, "bknight");
                    p.setImageDrawable(bknight);
                }
                else if(j == 6 && i == 0 || j == 1 && i == 0){
                    Drawable wknight =  getResources().getDrawable(R.drawable.wknight);
                    wknight.setBounds(xcoord, ycoord, xcoord+size, ycoord+size);
                    p = new Piece (getContext(), i, j, "wknight");
                    p.setImageDrawable(wknight);
                }
                //Bishops
                else if(j == 5 && i == 7 || j == 2 && i == 7){
                    Drawable bbishop =  getResources().getDrawable(R.drawable.bbishop);
                    bbishop.setBounds(xcoord, ycoord, xcoord+size, ycoord+size);
                    p = new Piece (getContext(), i, j, "bbishop");
                    p.setImageDrawable(bbishop);
                }
                else if(j == 5 && i == 0 || j == 2 && i == 0){
                    Drawable wbishop =  getResources().getDrawable(R.drawable.wbishop);
                    wbishop.setBounds(xcoord, ycoord, xcoord+size, ycoord+size);
                    p = new Piece (getContext(), i, j, "wbishop");
                    p.setImageDrawable(wbishop);
                }
                //Queen
                else if(j == 4 && i == 7){
                    Drawable bqueen =  getResources().getDrawable(R.drawable.bqueen);
                    bqueen.setBounds(xcoord, ycoord, xcoord+size, ycoord+size);
                    p = new Piece (getContext(), i, j, "bqueen");
                    p.setImageDrawable(bqueen);
                }
                else if(j == 4 && i == 0 ){
                    Drawable wqueen =  getResources().getDrawable(R.drawable.wqueen);
                    wqueen.setBounds(xcoord, ycoord, xcoord+size, ycoord+size);
                    p = new Piece (getContext(), i, j, "wqueen");
                    p.setImageDrawable(wqueen);
                }
                //Queen
                else if(j == 3 && i == 7){
                    Drawable bking =  getResources().getDrawable(R.drawable.bking);
                    bking.setBounds(xcoord, ycoord, xcoord+size, ycoord+size);
                    p = new Piece (getContext(), i, j, "bking");
                    p.setImageDrawable(bking);
                }
                else if(j == 3 && i == 0 ){
                    Drawable wking =  getResources().getDrawable(R.drawable.wking);
                    wking.setBounds(xcoord, ycoord, xcoord+size, ycoord+size);
                    p = new Piece (getContext(), i, j, "wking");
                    p.setImageDrawable(wking);
                }

                if (p != null) {
                    p.getDrawable().draw(canvas);
                    board[i][j].setPiece(p);
                    p.setOnTouchListener(this);
                }
                row.addView(board[i][j], j);
            }

            table.addView(row, i);
        }

        table.layout(0, 0, width, width);
        canvas.save();

        table.draw(canvas);
    }



    /*@RequiresApi(api = Build.VERSION_CODES.LOLLIPOP)
    @Override
    public void onDraw(Canvas canvas) {

        canvas.drawColor(Color.argb(100, 151, 182, 167));
        drawBoard(canvas);
        drawPiece(canvas);
    }*/


             /*   //
                public void drawBoard (Canvas canvas){
                    int width = getWidth();
                    int height = getHeight();
                    size = width / 8;
                    this.y0 = (height - size * 8) / 2;

                    for (int i = 0; i < rows; i++) {
                        TableRow row = new TableRow(this.context);
                        TableRow.LayoutParams lp = new TableRow.LayoutParams();
                        lp.height = size;

                        for (int j = 0; j < cols; j++) {
                            int xcoord = size * j;
                            int ycoord = y0 + size * i;

                            Piece p = new Piece(getContext(), j, i, "empty");

                            board[i][j] = new Tile(getContext(), j, i);
                            board[i][j].setPiece(p);
                            board[i][j].setSquare(rect);
                            board[i][j].setOnDragListener(this);
                            //board[i][j].setDrop(getContext());

                            rect.left = xcoord;
                            rect.top = ycoord;
                            rect.right = xcoord + size;
                            rect.bottom = ycoord + size;
                            board[i][j].draw(canvas);

                        }

                    }
                }

                public void drawPiece (Canvas canvas){

                }


                //Drawing the board
    /*public void drawBoard (Canvas canvas) {
        int width = getWidth();
        int height = getHeight();
        size = width/8;
        this.y0 = (height - size*8)/2;

        for (int i = 0; i < rows ; i ++) {
            for (int j = 0; j < cols; j++) {
                int xcoord = size * j;
                int ycoord = y0 + size * i;

                Piece p = new Piece(getContext(), j, i, "empty");

                board[i][j] = new Tile(getContext(), j, i);
                board[i][j].setPiece(p);
                board[i][j].setSquare(rect);
                board[i][j].setOnDragListener(this);
                //board[i][j].setDrop(getContext());

                rect.left = xcoord;
                rect.top = ycoord;
                rect.right = xcoord + size;
                rect.bottom = ycoord + size;
                board[i][j].draw(canvas);

            }

        }
    }

    public void drawPiece (Canvas canvas){

        int width = getWidth();
        int height = getHeight();
        size = width/8;
        this.y0 = (height - size*8)/2;

        for (int i = 0; i < cols ; i ++) {
            for (int j = 0; j < rows; j++) {
                int xcoord = size * (7 - i);
                int ycoord = y0 + size * (7 - j);
                //Setting the pieces
                piece_board[i][j] = new ImageView(getContext());
                //Pawn placement
                if(j==1){
                    Drawable wpawn = getResources().getDrawable(R.drawable.wpawn);
                    wpawn.setBounds(xcoord, ycoord, xcoord+size, ycoord+size);
                    piece_board[i][j].setImageDrawable(wpawn);
                    piece_board[i][j].getDrawable().draw(canvas);
                    Piece p = new Piece (getContext(), i, j, "wpawn");
                    board[i][j].setPiece(p);
                }
                else if(j==6){
                    Drawable bpawn = getResources().getDrawable(R.drawable.bpawn);
                    bpawn.setBounds(xcoord, ycoord, xcoord+size, ycoord+size);
                    piece_board[i][j].setImageDrawable(bpawn);
                    piece_board[i][j].getDrawable().draw(canvas);
                    Piece p = new Piece (getContext(), i, j, "bpawn");
                    board[i][j].setPiece(p);
                }
                //Rook
                else if(i == 7 && j == 7 || i == 0 && j == 7){
                    Drawable brook =  getResources().getDrawable(R.drawable.brook);
                    brook.setBounds(xcoord, ycoord, xcoord+size, ycoord+size);
                    piece_board[i][j].setImageDrawable(brook);
                    piece_board[i][j].getDrawable().draw(canvas);
                    Piece p = new Piece (getContext(), i, j, "brook");
                    board[i][j].setPiece(p);
                }
                else if(i == 7 && j == 0 || i == 0 && j == 0){
                    Drawable wrook =  getResources().getDrawable(R.drawable.wrook);
                    wrook.setBounds(xcoord, ycoord, xcoord+size, ycoord+size);
                    piece_board[i][j].setImageDrawable(wrook);
                    piece_board[i][j].getDrawable().draw(canvas);
                    Piece p = new Piece (getContext(), i, j, "wrook");
                    board[i][j].setPiece(p);
                }
                //Knights
                else if(i == 6 && j == 7 || i == 1 && j == 7){
                    Drawable bknight =  getResources().getDrawable(R.drawable.bknight);
                    bknight.setBounds(xcoord, ycoord, xcoord+size, ycoord+size);
                    piece_board[i][j].setImageDrawable(bknight);
                    piece_board[i][j].getDrawable().draw(canvas);
                    Piece p = new Piece (getContext(), i, j, "bknight");
                    board[i][j].setPiece(p);
                }
                else if(i == 6 && j == 0 || i == 1 && j == 0){
                    Drawable wknight =  getResources().getDrawable(R.drawable.wknight);
                    wknight.setBounds(xcoord, ycoord, xcoord+size, ycoord+size);
                    piece_board[i][j].setImageDrawable(wknight);
                    piece_board[i][j].getDrawable().draw(canvas);
                    Piece p = new Piece (getContext(), i, j, "wknight");
                    board[i][j].setPiece(p);
                }
                //Bishops
                else if(i == 5 && j == 7 || i == 2 && j == 7){
                    Drawable bbishop =  getResources().getDrawable(R.drawable.bbishop);
                    bbishop.setBounds(xcoord, ycoord, xcoord+size, ycoord+size);
                    piece_board[i][j].setImageDrawable(bbishop);
                    piece_board[i][j].getDrawable().draw(canvas);
                    Piece p = new Piece (getContext(), i, j, "bbishop");
                    board[i][j].setPiece(p);
                }
                else if(i == 5 && j == 0 || i == 2 && j == 0){
                    Drawable wbishop =  getResources().getDrawable(R.drawable.wbishop);
                    wbishop.setBounds(xcoord, ycoord, xcoord+size, ycoord+size);
                    piece_board[i][j].setImageDrawable(wbishop);
                    piece_board[i][j].getDrawable().draw(canvas);
                    Piece p = new Piece (getContext(), i, j, "wbishop");
                    board[i][j].setPiece(p);
                }
                //Queen
                else if(i == 4 && j == 7){
                    Drawable bqueen =  getResources().getDrawable(R.drawable.bqueen);
                    bqueen.setBounds(xcoord, ycoord, xcoord+size, ycoord+size);
                    piece_board[i][j].setImageDrawable(bqueen);
                    piece_board[i][j].getDrawable().draw(canvas);
                    Piece p = new Piece (getContext(), i, j, "bqueen");
                    board[i][j].setPiece(p);
                }
                else if(i == 4 && j == 0 ){
                    Drawable wqueen =  getResources().getDrawable(R.drawable.wqueen);
                    wqueen.setBounds(xcoord, ycoord, xcoord+size, ycoord+size);
                    piece_board[i][j].setImageDrawable(wqueen);
                    piece_board[i][j].getDrawable().draw(canvas);
                    Piece p = new Piece (getContext(), i, j, "wqueen");
                    board[i][j].setPiece(p);
                }
                //Queen
                else if(i == 3 && j == 7){
                    Drawable bking =  getResources().getDrawable(R.drawable.bking);
                    bking.setBounds(xcoord, ycoord, xcoord+size, ycoord+size);
                    piece_board[i][j].setImageDrawable(bking);
                    piece_board[i][j].getDrawable().draw(canvas);
                    Piece p = new Piece (getContext(), i, j, "bking");
                    board[i][j].setPiece(p);
                }
                else if(i == 3 && j == 0 ){
                    Drawable wking =  getResources().getDrawable(R.drawable.wking);
                    wking.setBounds(xcoord, ycoord, xcoord+size, ycoord+size);
                    piece_board[i][j].setImageDrawable(wking);
                    piece_board[i][j].getDrawable().draw(canvas);
                    Piece p = new Piece (getContext(), i, j, "wking");
                    board[i][j].setPiece(p);
                }

            }
        }

    }*/

                /*

    //Takes a list with all possible moves that a player can make, the move that player wants to make
    //Returns if the move is valid or not
    boolean isMoveValid(List<String> move_list, Move pmove){

        Piece p = mockMove(pmove);
        String presentBoard = boardToString();
        if (move_list.contains(presentBoard)){
            return true;
        }

        undoMove(pmove, p);
        return false;
    }

    //Get our present board and make it as a string
    String boardToString(){
        String boardMoves = "";

        for (int i = 0; i < cols ; i ++) {
            for (int j = 0; j < rows; j++) {
                boardMoves += board[i][j].getPiece().getName();
            }
        }
        return boardMoves;
    }

    //Apply a move to our board
    void applyMoveToBoard(Move move){
        Piece empty = new Piece (getContext(), "empty");
        board[move.getInit_col()][move.getInit_row()].setPiece(empty);
        board[move.getDest_col()][move.getDest_row()].setPiece(move.getPiece());
    }

    //Mock how the board would be if we applied the move
    //Returns the piece that was on the tile that we put our piece
    Piece mockMove(Move move){
        Piece empty = new Piece (getContext(), "empty");
        board[move.getInit_col()][move.getInit_row()].setPiece(empty);
        Piece ret = board[move.getDest_col()][move.getDest_row()].getPiece();
        board[move.getDest_col()][move.getDest_row()].setPiece(move.getPiece());
        return ret;
    }


    void undoMove(Move move, Piece p){
        //Set the intial piece back to its intial positions
        board[move.getInit_col()][move.getInit_row()].setPiece(move.getPiece());
        //Set the piece that used to be in the board back to its positions
        board[move.getDest_col()][move.getDest_row()].setPiece(p);
    }

    @Override
    public boolean onTouch(View v, MotionEvent event) {
        if (event.getAction() == MotionEvent.ACTION_DOWN) {
            ClipData data = ClipData.newPlainText("", "");
            View.DragShadowBuilder shadowBuilder = new View.DragShadowBuilder(
                    v);
            v.startDrag(data, shadowBuilder, v, 0);
            v.setVisibility(View.INVISIBLE);
            return true;
        } else {
            return false;
        }
    }

    @Override
    public boolean onDrag(View v, DragEvent event) {
        // Defines a variable to store the action type for the incoming event
        int action = event.getAction();
        // Handles each of the expected events
        switch (action) {

            case DragEvent.ACTION_DRAG_STARTED:
                // Determines if this View can accept the dragged data
                if (event.getClipDescription().hasMimeType(ClipDescription.MIMETYPE_TEXT_PLAIN)) {
                    // if you want to apply color when drag started to your view you can uncomment below lines
                    // to give any color tint to the View to indicate that it can accept data.
                    // v.getBackground().setColorFilter(Color.BLUE, PorterDuff.Mode.SRC_IN);
                    // Invalidate the view to force a redraw in the new tint
                    //  v.invalidate();
                    // returns true to indicate that the View can accept the dragged data.
                    return true;
                }
                // Returns false. During the current drag and drop operation, this View will
                // not receive events again until ACTION_DRAG_ENDED is sent.
                return false;

            case DragEvent.ACTION_DRAG_ENTERED:
                // Applies a GRAY or any color tint to the View. Return true; the return value is ignored.
                v.getBackground().setColorFilter(Color.GRAY, PorterDuff.Mode.SRC_IN);
                // Invalidate the view to force a redraw in the new tint
                v.invalidate();
                return true;

            case DragEvent.ACTION_DRAG_LOCATION:
                // Ignore the event
                return true;

            case DragEvent.ACTION_DRAG_EXITED:
                // Re-sets the color tint to blue. Returns true; the return value is ignored.
                // view.getBackground().setColorFilter(Color.BLUE, PorterDuff.Mode.SRC_IN);
                //It will clear a color filter .
                v.getBackground().clearColorFilter();
                // Invalidate the view to force a redraw in the new tint
                v.invalidate();
                return true;

            case DragEvent.ACTION_DROP:
                // Gets the item containing the dragged data
                ClipData.Item item = event.getClipData().getItemAt(0);
                // Gets the text data from the item.
                String dragData = item.getText().toString();
                // Turns off any color tints
                v.getBackground().clearColorFilter();
                // Invalidates the view to force a redraw
                v.invalidate();

                View vw = (View) event.getLocalState();
                ViewGroup owner = (ViewGroup) vw.getParent();
                owner.removeView(vw); //remove the dragged view
                //caste the view into LinearLayout as our drag acceptable layout is LinearLayout
                Tile container = (Tile) v;
                container.addView(vw);//Add the dragged view
                vw.setVisibility(View.VISIBLE);//finally set Visibility to VISIBLE
                // Returns true. DragEvent.getResult() will return true.
                return true;

            case DragEvent.ACTION_DRAG_ENDED:
                // Turns off any color tinting
                v.getBackground().clearColorFilter();
                // Invalidates the view to force a redraw
                v.invalidate();
                // returns true; the value is ignored.
                return true;
            // An unknown action type was received.
            default:
                Log.e("DragDrop Example", "Unknown action type received by OnDragListener.");
                break;
        }
        return false;
    }

    }






                 */