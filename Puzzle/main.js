var colors = ["FF0000", "00FF00", "0000FF", "FFFF00", "FF00FF", "00FFFF", "000000", 
        "800000", "008000", "000080", "808000", "800080", "008080", "808080", 
        "C00000", "00C000", "0000C0", "C0C000", "C000C0", "00C0C0", "C0C0C0", 
        "400000", "004000", "000040", "404000", "400040", "004040", "404040", 
        "200000", "002000", "000020", "202000", "200020", "002020", "202020", 
        "600000", "006000", "000060", "606000", "600060", "006060", "606060", 
        "A00000", "00A000", "0000A0", "A0A000", "A000A0", "00A0A0", "A0A0A0", 
        "E00000", "00E000", "0000E0", "E0E000", "E000E0", "00E0E0", "E0E0E0"]
/*
	var string = element.getAttribute('data-board');
	if(string.indexOf("(")>-1){
		string = string.replace("(","[");
		string = string.replace(")","]");
		string = "["+string;
		string = string+"]";
	}
*/

// Check for the various File API support.
if (window.File && window.FileReader && window.FileList && window.Blob) {
  console.log(solution);
} else {
  alert('The File APIs are not fully supported in this browser.');
}

function makePuzzleSVG(element){
	var gridSize = 20;
	
	/*State size is always the same so*/
	var width = solution[0].length;/*element.getAttribute('data-width');*/
	var height = solution[0][0].length;/*element.getAttribute('data-height');*/
	
	console.log("width: "+width);
	console.log("height: "+height);
	
	var states = Object.keys(solution).length;
	console.log("States: "+states);
	var i = 0;
	for (var state in solution){
		/*var puzzle = JSON.parse(string);*/
		/**/
		
		var node = document.createElement("svg"); 
		var id = "svg-"+i;
		node.setAttribute("width","400px");
		node.setAttribute("height","300px");
		node.setAttribute("id",id);
		element.appendChild(node);
		var sid = "#"+id;
		
		var s = Snap(120,100);/*100, 80);*/		
		var blocks = [];
		for(var w=0; w < width; w++){
			for(var h=0; h < height; h++){
				var loc = solution[i][w][h];
				console.log("Code at ["+w+"]["+h+"]: "+loc);
				var color = colors[loc];
				console.log("Color at "+loc+": "+color);
				blocks.push( s.rect(gridSize*w,gridSize*h,gridSize,gridSize) );
				blocks[blocks.length-1].attr({
					fill: "#"+color,
					stroke: "rgba(0,0,0,0.2)",
					strokeWidth: 1
				});
			}
		}
		i++;
	}
}

var frames = document.getElementsByClassName("content");
makePuzzleSVG(frames[0]);
