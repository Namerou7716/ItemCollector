# include <Siv3D.hpp> // Siv3D v0.6.15

/*
* Playerクラス
*/
class Player
{
private:
	double speed = 500.0;
	Texture playerTex{ U"☺"_emoji };
	Circle playerCollision{ 400,480,35 };
public:
	void draw()const
	{
		playerTex.scaled(0.5).drawAt(playerCollision.center);
	}

	void update(double deltaTime)
	{
		if (KeyD.pressed())
		{
			playerCollision.x += speed * deltaTime;
		}
		if (KeyA.pressed())
		{
			playerCollision.x -= speed * deltaTime;
		}
		playerCollision.x = Math::Clamp(playerCollision.x, 40, 760);
	}
};

/*
* アイテムクラス
*/
class Item
{
private:
	Circle itemCollision;
	int32 itemType;
public:
	Item()
	{
		itemCollision = Circle{ Random(70,730),20,35 };
		itemType = Random(0, 1);
	}
	void draw()const
	{
		switch (this->itemType)
		{
		default:
			break;
		case 0:
			itemCollision.draw(Palette::White);
			break;
		case 1:
			itemCollision.draw(Palette::Red);
			break;
		}
	}
};

/*
* 背景描画
*/
void DrawBackground()
{
	Rect sky{ 0,0,800,500 };
	Rect ground{ 0,500,800,100 };
	sky.draw(Arg::top = Palette::Deepskyblue, Arg::bottom = Palette::Aqua);
	ground.draw(Palette::Mediumseagreen);
}

/*
* アイテムをまとめて描画する関数
*/
void DrawItems(const Array<Item>& items)
{
	for (const auto& item : items)
	{
		item.draw();
	}
}
void Main()
{
	Player player;
	Array<Item>items;
	items << Item{};
	items << Item{};

	while (System::Update())
	{
		/*
		*
		*
		*更新
		*
		*/
		player.update(Scene::DeltaTime());
		/*
		*
		*
		* 描画
		*
		*/
		DrawBackground();
		player.draw();
		DrawItems(items);
	}
}

