class GameObject {

	GameObject() {};
	~GameObject() {};

	void Update();

public:
	char name[32] = "";


};

class Component {

	Component() {};
	~Component() {};

	void CreateComponent() {};

public:
	enum type;
	bool active;
	
};