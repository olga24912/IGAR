/*
*Handle zooming
*/

//Create zoom drop down menu
function zoomDropDown() {
    document.getElementById("zoomDropdown").classList.toggle("show");
}

//Handle zoom +1.5X
function zoomPlus() {
    if (cy !== null) {
        var width =  document.getElementById('mainpanel').clientWidth;
        var height =  document.getElementById('mainpanel').clientHeight;
        var changeIn = Math.min(cy.zoom()*1.5, cy.maxZoom())/cy.zoom();

        cy.panBy({x: -(width - width/changeIn)/2, y: -(height - height/changeIn)/2});
        cy.pan({x: cy.pan().x * changeIn, y: cy.pan().y * changeIn});
        cy.zoom(cy.zoom() * changeIn);
        if ((document.getElementById("select_layout").value !== "free_layout")) {
            cy.pan({x:cy.pan().x, y: height/3});
        }
    }
}

//Handle zoom -1.5X
function zoomMinus() {
    if (cy !== null) {
        var width =  document.getElementById('mainpanel').clientWidth;
        var height =  document.getElementById('mainpanel').clientHeight;
        var changeIn = Math.max(cy.zoom()/1.5, cy.minZoom())/cy.zoom();

        cy.panBy({x: -(width - width/changeIn)/2, y: -(height - height/changeIn)/2});
        cy.pan({x: cy.pan().x * changeIn, y: cy.pan().y * changeIn});
        cy.zoom(cy.zoom() * changeIn);
        if ((document.getElementById("select_layout").value !== "free_layout")) {
            cy.pan({x: cy.pan().x, y: height/3});
        }
        //cy.pan({x: 0, y: -100});
    }
}

// Close the dropdown menu if the user clicks outside of it
window.onclick = function(event) {
    if (!event.target.matches('.zoomSelect')) {
        var dropdowns = document.getElementsByClassName("dropdown-content");
        var i;
        for (i = 0; i < dropdowns.length; i++) {
            var openDropdown = dropdowns[i];
            if (openDropdown.classList.contains('show')) {
                openDropdown.classList.remove('show');
            }
        }
    }
};


//Handle Alt- and Alt=
window.addEventListener("keyup", function (evt) {
    if (evt.altKey) {
        var code = (evt.keyCode || evt.which);
        if (code === 187) {
            zoomPlus();
        } else if (code === 189) {
            zoomMinus();
        }

        if (evt.ctrlKey) {
            if (evt.keyCode === 69) {
                var imgBlob = cy.png({
                    output: 'blob',
                    bg: "#ffffff",
                    quality: 1,
                    scale: 5
                });
                //var imgBlob = base64ToBlob( b64, 'image/svg' );

                saveAs(imgBlob, 'graph.png');
            }
        }
    }
});


//Handle array manipulation
window.addEventListener("keydown", function (evt) {
    var inputs = ['input', 'select', 'button', 'textarea'];

    if (document.activeElement &&
        inputs.indexOf(document.activeElement.tagName.toLowerCase()) !== -1) {
        return;
    }

    var delta = 20;
    if (evt.shiftKey) {
        delta = 100;
    }
    var code = (evt.keyCode || evt.which);
    if (code === 37) { //left arrow
        cy.panBy({
            x: delta,
            y:0
        });
    } else if (code === 39) { //right arrow
        cy.panBy({
            x:-delta,
            y:0
        });
    } else if (code === 38) { //up arrow
        cy.panBy({
            x:0,
            y:delta
        });
    } else if (code === 40) { //down arrow
        cy.panBy({
            x:0,
            y:-delta
        });
    }
});

//Handle zooming from input
function updateZoomFromInput() {
    var inputVal = parseInt(document.getElementById("zoomInput").innerText);

    if (cy !== null) {
        var width =  document.getElementById('mainpanel').clientWidth;
        var height =  document.getElementById('mainpanel').clientHeight;
        if (inputVal > 10000) {
            inputVal = 9999;
        }
        if (inputVal < 0) {
            inputVal = 1;
        }

        var changeIn = (inputVal*defZoom/(100 * 100))/cy.zoom();

        cy.panBy({x: -(width - width/changeIn)/2, y: -(height - height/changeIn)/2});
        cy.pan({x: cy.pan().x * changeIn, y: cy.pan().y * changeIn});
        cy.zoom(cy.zoom() * changeIn);
        if ((document.getElementById("select_layout").value !== "free_layout")) {
            cy.pan({x: cy.pan().x, y: height/3});
        }

        (document.getElementById("zoomInput")).innerText = (cy.zoom() * 100 * 100/defZoom).toString() +  "%";

        if ((document.getElementById("select_layout").value !== "free_layout")) {
            cy.pan({x: cy.pan().x, y: height/3});
        }
    } else {
        (document.getElementById("zoomInput")).innerText = "100%";
    }
}

document.getElementById("zoomInput").addEventListener("focusout", updateZoomFromInput);

document.getElementById("zoomInput").addEventListener("keypress", function (e) {
    //if enter press
    if (e.keyCode === 13) {
        document.getElementById("zoomInput").blur();
        updateZoomFromInput()
    }
});