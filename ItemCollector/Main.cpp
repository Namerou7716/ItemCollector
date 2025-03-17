# include <Siv3D.hpp> // Siv3D v0.6.15

/*
* アイテムクラス
*/
class Item
{
private:
	int32 itemType;
	double fallSpeed{ 200 };
public:
	Circle itemCollision;
	Vec2 position;
public:
	Item()
	{
		position = { Random(70,730),20 };
		itemCollision = Circle{ position,35 };
		itemType = Random(0, 1);
	}
	void draw(const Array<Texture>itemTextures)const
	{
		itemTextures[itemType].scaled(0.5).draw(itemCollision.center);
	}
	void update(double deltaTime)
	{
		position.y += fallSpeed * deltaTime;
		itemCollision.setPos(position);
	}
};

/*
* Playerクラス
*/
class Item;
class Player
{
private:
	double speed = 500.0;
	Texture playerTex{ U"☺"_emoji };
public:
	int32 score{ 0 };
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
	/*
	* Playerとアイテムの交差判定
	*/
	bool IsTouchItem(const Item& item)const
	{
		return playerCollision.intersects(item.itemCollision);
	}
};
/*
* 経過時間を確認できるタイマークラス
*/
class PeriodTimer
{
private:
	double accumelateTime{ 0 };
	double periodTime{ 0 };

public:
	PeriodTimer(double time)
	{
		periodTime = time;
	}
	void updateTime(double deltaTime)
	{
		accumelateTime += deltaTime;
	}

	bool checkAccumelate()
	{
		if (accumelateTime >= periodTime)
		{
			accumelateTime = 0.0;
			return true;
		}
		return false;
	}

};
/*
* 背景描画関数
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
void DrawItems(const Array<Item>& items, const Array<Texture>& itemTextures)
{
	for (const auto& item : items)
	{
		item.draw(itemTextures);
	}
}
/*
* アイテムの更新関数
*/
void UpdateItems(Array<Item>& items, double deltaTime, Player& player)
{
	for (auto& item : items)
	{
		item.update(deltaTime);
	}
	//プレイヤーと交差したら消す
	items.remove_if([&](const Item& item) {return player.IsTouchItem(item); });
	//地面についたら消す
	items.remove_if([](Item item) { return item.position.y > 450; });
}

/*
* UI描画関数
*/
void DrawUI(Font font, int32 score)
{
	font(U"Score : {}"_fmt(score)).draw(Arg::topLeft(10, 10), ColorF(0.2));
}

void Main()
{
	Player player;
	Array<Item>items;
	Array<Texture>itemTextureList{ Texture{U"🍬"_emoji},Texture{U"🍰"_emoji } };
	PeriodTimer timer{ 0.8 };
	Font font{ FontMethod::MSDF,40,Typeface::Bold };

	while (System::Update())
	{
		const double deltaTime = Scene::DeltaTime();
		/*
		*更新
		*/
		player.update(deltaTime);
		UpdateItems(items, deltaTime, player);
		timer.updateTime(deltaTime);
		if (timer.checkAccumelate())
		{
			items << Item{};
		}
		/*
		* 描画
		*/
		DrawBackground();
		player.draw();
		DrawItems(items, itemTextureList);
		DrawUI(font, player.score);
	}
}

