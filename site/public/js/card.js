const cards = document.getElementsByClassName("card");
const subpage = document.getElementById("subpage");

for (let i = 0; i < cards.length; i++)
{
	cards[i].addEventListener("click", (e) =>
	{
		fetch(`/getcard/${cards[i].dataset.id}`).then((response) => response.text()).then((data) => subpage.innerHTML = data);
	});
}

cards[0].click();
