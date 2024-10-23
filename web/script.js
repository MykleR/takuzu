var canvas = document.getElementById('takuzu');
var ctx = canvas.getContext('2d');
let g;
let gridCell = new Image();
let imBlack = new Image();
let imBlue = new Image();
let black = new Image() ;
let blue = new Image();
let redCross = new Image();

gridCell.src = '../images/gridCell4.png';
imBlack.src = '../images/imutable_black.png';
imBlue.src = '../images/imutable_blue.png';
black.src = '../images/black.png';
blue.src = '../images/blue.png';
redCross.src = '../images/red_cross.png';

ctx.save();
ctx.restore();

function printGame() {
    var nb_rows = Module._nb_rows(g);
    var nb_cols = Module._nb_cols(g);
    ctx.clearRect(0,0,canvas.width,canvas.height);
    var cellSize = canvas.width/ nb_cols;

    for (var row = 0; row < nb_rows; row++) {
        for (var col = 0; col < nb_cols; col++) {
            var number = Module._get_number(g, row, col);
            var immutable = Module._is_immutable(g, row, col);
            var x = col*cellSize;
            var y = row*cellSize;
            ctx.drawImage(gridCell, col * cellSize, row * cellSize, cellSize, cellSize);
            
            if (immutable && number == 0)
                ctx.drawImage(imBlue,x, y, cellSize, cellSize);
            else if (immutable && number == 1)
                ctx.drawImage(imBlack,x, y, cellSize, cellSize);
            else if (!(immutable) && number == 0)
                ctx.drawImage(blue,x, y, cellSize, cellSize);
            else if (!(immutable) && number == 1)
                ctx.drawImage(black,x, y, cellSize, cellSize);

            if(Module._has_error(g,row,col))
                ctx.drawImage(redCross,x, y, cellSize, cellSize);
        }
    }
    

    if (Module._is_over(g)==true){
        var elm = document.getElementById('win');
        elm.innerHTML = "Victory";
    }
}

function reset(){
    document.getElementById("reset").addEventListener("click", function(){
        Module._restart(g);
        printGame();
    });
}

function solve(){
    document.getElementById("solve").addEventListener("click" , function(){
        Module._restart(g);
        Module._solve(g);
        printGame();
    });
}

function random() {
    document.getElementById("random").addEventListener("click" , function(){
        Module._delete(g);
        var uniq = document.getElementById("uniq");
        var unstate = uniq.checked;
        var wrap = document.getElementById("wrap");
        var wrstate = wrap.checked;
        var hardness = document.getElementById("hard-select");
        var difficulty = hardness.value;
        if (difficulty == 'easy')
            g = Module._new_random(6,6,unstate,wrstate);
        else if (difficulty == 'medium')
            g =  Module._new_random(8,8,unstate,wrstate);
        else
            g = Module._new_random(10,10,unstate,wrstate);
        printGame();
    });

}

function undo(){
    document.getElementById("undo").addEventListener("click" , function(){
        Module._undo(g);
        printGame();
    });
}

function redo(){
    document.getElementById("redo").addEventListener("click" , function(){
        Module._redo(g);
        printGame();
    });
}



function getSquareOnClick (canvas) {
    
    canvas.addEventListener("click", function(event) {
        var rect = canvas.getBoundingClientRect();
        var x = event.clientX - rect.left;
        var y = event.clientY - rect.top;
        var nb_rows = Module._nb_rows(g);
        var cellSize = canvas.width/nb_rows;
        var squareX = Math.trunc(x / cellSize);
        var squareY = Math.trunc(y / cellSize);

        var sq_num = Module._get_number(g, squareY, squareX);
        if (Module._is_immutable(g,squareY,squareX)==true)
            return;
        switch(sq_num){
            case 0 :
                Module._check_move(g,squareY,squareX)
                Module._play_move(g, squareY, squareX, 2);
                break;
            case 1 :
                Module._play_move(g, squareY, squareX, 0);
                break;
            case -1 :
                Module._play_move(g, squareY, squareX, 1);
        }
        printGame();
    });
}

function start() {
    console.log("call start routine");
    g = Module._new_default();
    printGame();
    // Module._play_move(g, 3, 0, 1);
    getSquareOnClick(canvas);
    reset();
    random();
    solve();
    undo();
    redo();
    // Module._delete(g);w
}

Module.onRuntimeInitialized = () => { start(); }

