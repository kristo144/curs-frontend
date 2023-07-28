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

function chooseCard(card) {
	flipCard(card);
}
