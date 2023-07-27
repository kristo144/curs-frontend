var fullDeck = null;
var firstCard = null;
var secondCard = null;

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

	firstCard = null;
	secondCard = null;
	document.querySelector(".info-msg").innerHTML = "Pick 2 cards!";
}

function chooseCard(card) {
	flipCard(card);
	if (firstCard == null) {
		firstCard = card;
	}
	else {
		secondCard = card;
		compareCards(firstCard, secondCard);
		disableGame();
	}
}

function compareCards(f, s) {
	let winner = f;
	let loser = s;
	let attack = parseInt(f.querySelector(".card-attack").innerHTML);
	let defense = parseInt(s.querySelector(".card-defense").innerHTML);

	if (attack <= defense) {
		[winner, loser] = [s, f];
	}

	setTimeout(showResults, 2000, winner, loser);
}

function showResults(winner, loser) {
	winner.classList.add("card-winner");
	loser.classList.add("card-loser");
	document.querySelector(".info-msg").innerHTML =
		winner.querySelector(".card-name").innerHTML +
		" defeats " +
		loser.querySelector(".card-name").innerHTML;
}


function disableGame() {
	for (card of document.querySelectorAll(".card")) {
		card.onclick = null;
	}
}
