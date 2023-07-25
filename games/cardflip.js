var fullDeck = null;

async function playGame() {
	let input = document.querySelector('#cardsNumber');

	if (fullDeck == null) {
		fullDeck = await getFullDeck();
		input.max = fullDeck.length;
	}
	shuffleDeck(fullDeck);

	let n = input.value;
	let newCards = fullDeck.toSpliced(0, fullDeck.length - n);

	clearCards();
	for (c of newCards) {
		addCard(c);
	}
}

function getFullDeck() {
	return fetch("../assets/dbz.json").then(data => data.json());
}

// https://en.wikipedia.org/wiki/Fisher-Yates_shuffle
function shuffleDeck(d) {
	l = d.length - 1;
	while (l >= 0) {
		k = randInt(0, l);
		[d[k], d[l]] = [d[l], d[k]]; // swap items
		l--;
	}
}

function randInt(min, max) {
	return Math.floor(Math.random() * (max - min + 1)) + min;
}


function clearCards() {
	let cards = document.querySelectorAll(".card-container > article.card");
	for (c of cards) {
		c.remove();
	}
}

function addCard(c) {
	const tpl = document.getElementById('card-template');
	let newCard = tpl.content.cloneNode(true);
	setCardProperties(newCard, c);
	document.querySelector(".card-container").appendChild(newCard);
}

// UGLY!!
function setCardProperties(card, data) {
	card.querySelector(".card").onclick = function() { flipCard(this); };
	card.querySelector(".card-name").textContent = data.name;
	card.querySelector(".card-attack").textContent = data.attack;
	card.querySelector(".card-characteristic-attack").textContent = data.characteristic_attack;
	card.querySelector(".card-defense").textContent = data.defense;
	card.querySelector(".card-id").textContent = data.id;
	card.querySelector(".card-race").textContent = data.race;
	card.querySelector(".card-image").src = "../assets/img/" + data.image;
}

function flipCard(card) {
	let classes = card.classList;
	classes.add("card-flipping");
	card.addEventListener(getTransitionEndEventName(), changeCardState);
}

function changeCardState(event) {
	let card = event.target
	let classes = card.classList;
	// faceup overrides facedown
	if (classes.contains("card-faceup")) {
		classes.remove("card-faceup");
	}
	else {
		classes.add("card-faceup");
	}
	classes.remove("card-flipping");
	card.removeEventListener(getTransitionEndEventName(), changeCardState);
}

// get name of end transition event on different browsers
function getTransitionEndEventName() {
  var transitions = {
      "transition"      : "transitionend",
      "OTransition"     : "oTransitionEnd",
      "MozTransition"   : "transitionend",
      "WebkitTransition": "webkitTransitionEnd"
   }
  let bodyStyle = document.body.style;
  for(let transition in transitions) {
      if(bodyStyle[transition] != undefined) {
          return transitions[transition];
      } 
  }
}
