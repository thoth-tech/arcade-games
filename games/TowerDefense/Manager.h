#ifndef _MANAGER_H_
#define _MANAGER_H_

template <typename T>
class Manager
{
public:
	static T* instance()//外界取得钥匙的方法，从instance员工这里来拿
	{
		if (!manager)
			manager = new T();//如果之前还没有钥匙，创建钥匙

		return manager;//刚创建好钥匙/已经有钥匙，直接返回钥匙
	
	}

private:
	static T* manager;//钥匙实例

protected:
	Manager() = default;//创建钥匙方法
	~Manager() = default;
	Manager(const Manager&) = delete;
	Manager& operator=(const Manager&) = delete;

};
template <typename T>//占位模板，调用时编译器根据你输入的T把你模板里写的T全换成你输入的那个
T* Manager<T>::manager = nullptr;
#endif // !_MANAGER_H_

/*
关于理解可继承单例模板：是一份天生该只有一份并被全局共享的实例，也就是说他是特定的，后面所有想使用它的地方都直接从这个地方拿特殊的就行了，比如说你想拿公司总账对账那个，不管在哪个地方拿公司账本他应该都是
同一份，在这里就是一个实例，而不是两份账本也就是两个实例，这样就会产生错误，所以当你以后需要公司账本时，你直接来可继承单例模板这个部门直接拿就行了，不需要你传入参数等等，只需要直接拿，方便快捷而且不易出错
优点还有很多，这里不再赘述*/

/*
关于实现可继承单例模板：instance方法作为manager类的内部员工，它可以搞到内部manager的方法和信息，所以如果要做可继承单例模板，我们就让其他想要钥匙的人都来找他，他就可以把做好的同一把特殊钥匙分发给其他人，
而其他人不能直接通过类来造钥匙而只能来找这个员工拿，这个钥匙就是我们想要实现的功能，所以我们每个人从可继承单例模板拿到的都是同样特殊的东西
*/
