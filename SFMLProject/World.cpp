#include <Book/World.hpp>

#include <SFML/Graphics/RenderWindow.hpp>
#include "Book/Utility.hpp"
#include <iostream>

struct point
{
	int x, y;
};

int count = 0;

int x = 0, y = 0, h = 200;
float dx = 0, dy = 0;

point plat[20];



World::World(sf::RenderWindow& window)
	: mWindow(window)
	, mWorldView(window.getDefaultView())
	, mTextures()
	, mSceneGraph()
	, mSceneLayers()
	, mWorldBounds(0.f, 0.f, mWorldView.getSize().x, 533.f)
	, mSpawnPosition(mWorldView.getSize().x / 2.f, mWorldBounds.height - mWorldView.getSize().y / 2.f)
	, mScrollSpeed(-50.f)
	, mDoodle(nullptr)
	, mPlatform()
	, sPlat()
	, t2()
{
	loadTextures();
	buildScene();
	t2.loadFromFile("Media/Textures/platform.png");
	sPlat.setTexture(t2);
	for (int i = 0; i < 10; i++) //'randomizes' where the platforms are
	{
		plat[i].x = rand() % 400;
		plat[i].y = rand() % 533;
	}

	// Prepare the view

	mWorldView.setCenter(mSpawnPosition);
}

CommandQueue& World::getCommandQueue()
{
	return mCommandQueue;
}




void World::update(sf::Time dt)
{
	// Scroll the world
	//mWorldView.move(0.f, mScrollSpeed * dt.asSeconds());	
	//mDoodle->setVelocity(0.0f, 0.0f);
	dy += 0.2;
	y += dy;

	x = mDoodle->getPosition().x;
	mDoodle->setVelocity(0, mDoodle->getVelocity().y);
	if (y > 533 - 40) //if player falls to the bottom of the screen
	{
		dy = -10;
	}



	for (int i = 0; i<10; i++)//if player hits platform
	{
		if ((x + 40 > mPlatform[i]->getPosition().x) &&
			(x - 10 < mPlatform[i]->getPosition().x + 68) &&
			(y + 40 > mPlatform[i]->getPosition().y) &&
			(y + 40 < mPlatform[i]->getPosition().y + 12) && (dy > 0))
		{
			dy = -10;
		}


	}

	if (y < h) //if player pos is higher than the screen
		for (int i = 0; i < 10; i++)
		{
			y = h;
			plat[i].y -= dy;
			if (plat[i].y > 533)
			{
				plat[i].y = 0; plat[i].x = rand() % 400;
				//mPlatform[i]->setPosition(rand() % 400, 0);
			}
		}

	for (int i = 0; i < 10; i++)
	{
		mPlatform[i]->setPosition(plat[i].x, plat[i].y);
	}

	//std::cout << "Player's World Position: " << worldpos.x << ", " << worldpos.y << " Player's Position: " << pos.x << ", " << pos.y << std::endl;

	// Forward commands to scene graph, adapt velocity (scrolling, diagonal correction)
	while (!mCommandQueue.isEmpty())
	{
		mSceneGraph.onCommand(mCommandQueue.pop(), dt);
	}

#pragma region step 5

	adaptPlayerVelocity();
	mDoodle->setPosition(x, y);

	// Regular update step, adapt position (correct if outside view)
	mSceneGraph.update(dt);
	adaptPlayerPosition();

#pragma endregion
}

void World::draw()
{
	mWindow.setView(mWorldView);
	mWindow.draw(mSceneGraph);

}

void World::loadTextures()
{
	mTextures.load(Textures::DoodlePlayer, "Media/Textures/DoodlePlayer.png");
	mTextures.load(Textures::platform, "Media/Textures/platform.png");
	mTextures.load(Textures::background, "Media/Textures/background.png");
}

void World::buildScene()
{
	// Initialize the different layers
	for (std::size_t i = 0; i < LayerCount; ++i)
	{
		SceneNode::Ptr layer(new SceneNode());
		mSceneLayers[i] = layer.get();

		mSceneGraph.attachChild(std::move(layer));
	}

	// Prepare the tiled background
	sf::Texture& texture = mTextures.get(Textures::background); //setting the background sprite
	sf::IntRect textureRect(mWorldBounds);
	texture.setRepeated(true);

	// Add the background sprite to the scene
	std::unique_ptr<SpriteNode> backgroundSprite(new SpriteNode(texture, textureRect));
	backgroundSprite->setPosition(mWorldBounds.left, mWorldBounds.top);
	mSceneLayers[Background]->attachChild(std::move(backgroundSprite));



	// Add player's aircraft
	std::unique_ptr<Doodle> leader(new Doodle(Doodle::DoodlePlayer, mTextures));
	mDoodle = leader.get();
	mDoodle->setPosition(mSpawnPosition);
	mSceneLayers[Air]->attachChild(std::move(leader));
	mDoodle->setVelocity(0.f, 0.f);

	//add platform
	for (int i = 0; i < 20; i++)
	{
		std::unique_ptr<Platform> myplat(new Platform(Platform::platform, mTextures));
		mPlatform[i] = myplat.get();
		mSceneLayers[Air]->attachChild(std::move(myplat));

	}

	// Add two escorting aircrafts, placed relatively to the main plane
	/*std::unique_ptr<Aircraft> leftEscort(new Aircraft(Aircraft::Raptor, mTextures));
	leftEscort->setPosition(-80.f, 50.f);
	mDoodle->attachChild(std::move(leftEscort));

	std::unique_ptr<Aircraft> rightEscort(new Aircraft(Aircraft::Raptor, mTextures));
	rightEscort->setPosition(80.f, 50.f);
	mDoodle->attachChild(std::move(rightEscort));*/
}

#pragma region step 4

void World::adaptPlayerPosition()
{
	// Keep player's position inside the screen bounds, at least borderDistance units from the border
	sf::FloatRect viewBounds(mWorldView.getCenter() - mWorldView.getSize() / 2.f, mWorldView.getSize());
	const float borderDistance = 33.f;
	const float maxVel = 100.f;

	sf::Vector2f position = mDoodle->getPosition();
	position.x = std::max(position.x, viewBounds.left + borderDistance);
	position.x = std::min(position.x, viewBounds.left + viewBounds.width - borderDistance);

	//sf::Vector2f clampedPos = mDoodle->getVelocity();
	//clampedPos.x = util.Clamp(mDoodle->getVelocity().x, -200.f, 200.f);
	//clampedPos = sf::Vector2f(0, mDoodle->getVelocity().y);
	//mDoodle->setVelocity(clampedPos);


	//position.y = std::max(position.y, viewBounds.top + borderDistance);
	//position.y = std::min(position.y, viewBounds.top + viewBounds.height - borderDistance);
	mDoodle->setPosition(position);
}

void World::adaptPlayerVelocity()
{
	sf::Vector2f velocity = mDoodle->getVelocity();

	// If moving diagonally, reduce velocity (to have always same velocity)
	if (velocity.x != 0.f && velocity.y != 0.f)
		mDoodle->setVelocity(velocity / std::sqrt(2.f));

	// Add scrolling velocity
	//mDoodle->accelerate(0.f, 300.f);



}

#pragma endregion

