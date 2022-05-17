#include <map>
// ----------------------------------------------------------------------
#define PRINT(msg)		LOGMSG(msg) {std::stringstream ss; ss << msg << "\r\n"; printf(ss.str().c_str());}
#define PRINTVEC(vm, v)	PRINT(vm << " = " << v.x << "," << v.y << "," << v.z)

#define MYTRACE(msg)	{std::stringstream ss; ss << msg << "\r\n"; printf(ss.str().c_str()); ::OutputDebugStringA(ss.str().c_str());}
#define ASSERT(a)		if(!(a)) throw;
#define LOGMSG(msg)		{if(gblog) logmessage << msg << "\r\n";}
#define event(obj, msg)	{if(gblog) logmessage << obj << " : " << msg << "\r\n";}
#define ASSERT(x)		{if(!(x)){LOGMSG(__FILE__ << "(" << __LINE__ << ")"); throw;} }
// ----------------------------------------------------------------------
#define d100				(rand() % 100 + 1)
#define d6					(rand() % 6 + 1)
#define dx(x)				(rand() % x + 1)	
// ----------------------------------------------------------------------
bool gblog = true;
std::stringstream logmessage;
// **********************************************************************************************
// DataModel
// **********************************************************************************************
// ItemData
// ----------------------------------------------------------------------------------------------
struct PlayerData;
struct ItemData
{
	int id;
	int img;
	int price;

	ItemData()
	{
		id = 0;
		img = 0;
		price = 0;
	}
	virtual bool use(PlayerData& pd) = 0;
};

struct ItemDataMgr
{	
	static ItemDataMgr inst;
	std::map<int, ItemData*> list;
	ItemData* findItemData(int id)
	{
		std::map<int, ItemData*>::iterator it = list.find(id);
		if (it != list.end())
		{
			return it->second;
		}
		return 0;
	}
};
ItemDataMgr ItemDataMgr::inst;
// ----------------------------------------------------------------------------------------------
// PlayerData
// ----------------------------------------------------------------------------------------------
enum{ EQUIP_HEAD, EQUIP_NECK, EQUIP_CHEST, EQUIP_LEG, EQUIP_HAND, EQUIP_FINGER, EQUIP_WEAPON, EQUIPNUM };
#define INVALID_BAGIND	-1
struct PlayerData
{
	// basic property
	int level;
	int exp;
	int hp;
	int maxhp;
	int mp;
	int maxmp;
	int money;

	// basic property
	int strength;
	int agility;
	int magic;
	int diankang;
	int huokang;
	int bingkang;
	int dukang;

	// extern property
	int attack;
	int defence;
	real attackinterval;
	int gedang;
	int xixue;
	int mingzhong;
	int baoji;
	int diangong;
	int huogong;
	int binggong;
	int dugong;

	// ս��״̬
	int nuqi;
	int skillid;
	int monsterBDplyer;
	
	//equipment
	int equipment[EQUIPNUM];

	PlayerData()
	{
		// basic property
		level = 1;
		exp = 0;
		hp = 100;
		maxhp = 10;
		mp = 12;
		maxmp = 0;
		money = 100;

		// property
		strength = 2;
		agility = 5;
		magic = 0;
		diankang = 0;
		huokang = 0;
		bingkang = 0;
		dukang = 0;

		attack = 12;
		defence = 5;
		attackinterval = 0.686;
		gedang = 0;
		xixue = 0;
		mingzhong = 50;
		baoji = 0;
		diangong = 0;
		huogong = 0;
		binggong = 0;
		dugong = 0;

		nuqi = 0;
		skillid = 1;
		monsterBDplyer = 0;

		memset(equipment, 0, sizeof(equipment));

	}

	int getBaseStrength()
	{
		return 10 + level - 1;
	}
	int getStrength()
	{
		return getBaseStrength() + strength;
	}
	int getBaseAgility()
	{
		return 8 + level - 1;
	}
	int getAgility()
	{
		return getBaseAgility() + agility;
	}
	int getBaseMagic()
	{
		return 2 + level - 1;
	}
	int getMagic()
	{
		return getBaseMagic() + magic;
	}
	int getBaseMaxHP()
	{
		return getStrength() + 100;
	}
	int getMaxHP()
	{
		return getBaseMaxHP() + maxhp;
	}
	int getBaseMaxMP()
	{
		return getMagic() + 100;
	}
	int getMaxMP()
	{
		return getBaseMaxMP() + maxmp;
	}	
	int getMaxExp()
	{
		return 6 * level + 4;
	}
	int getBaseAttack()
	{
		return getStrength() - 5;
	}
	int getBaseDefence()
	{
		return getAgility() - 8;
	}
	real getBaseAttackInterval()
	{
		return 2 - 0.04 * getAgility();
	}
	int getBaseGeDang()
	{
		return 4 + getAgility() / 4.0;
	}
	int getGeDang()
	{
		return 4 + getAgility() / 4.0 + gedang;
	}
	int getAttck()
	{
		return getBaseAttack() + attack;
	}
	int getDefence()
	{
		return getBaseDefence() + defence;
	}
	real getAttackinterval()
	{
		return getBaseAttackInterval() + attackinterval;
	}
	int getBaoJi()
	{
		return baoji + 0;
	}
	int getMingZhong()
	{
		return mingzhong + 11;
	}
	void addExp(int dexp)
	{	
		//dexp *= 10;
		exp += dexp;
		if (exp >= getMaxExp())
		{
			level++;
			exp -= getMaxExp();
			PRINT("player levelup : " << level)
		}
		else
		{
			PRINT("player addExp : " << dexp)
		}
	}
};
// ----------------------------------------------------------------------------------------------
// HPMed
// ----------------------------------------------------------------------------------------------
struct HPMed : public ItemData
{
	int hp;
	HPMed()
	{
		hp = 10;
		id = 100;
		img = 5;
	}
	virtual bool use(PlayerData& pd)
	{
		pd.hp += hp;
		event("player", "usehpmed");
		return true;
	}
};
// ----------------------------------------------------------------------------------------------
// MPMed
// ----------------------------------------------------------------------------------------------
struct MPMed : public ItemData
{
	int mp;
	MPMed()
	{
		price = 10;
		id = 101;
		img = 5;
	}
	virtual bool use(PlayerData& pd)
	{
		pd.mp += mp;
		event("player", "usempmed");
		return true;
	}
};
// ----------------------------------------------------------------------------------------------
// Ore
// ----------------------------------------------------------------------------------------------
struct Ore : public ItemData
{
	int tichun;
	int tichunnum;
	Ore()
	{
		tichun = 0;
		tichunnum = 0;
		price = 10;
		id = 88;
	}
	virtual bool use(PlayerData& pd)
	{
		PRINT("��ʯ����ֱ��ʹ��!");
		return false;
	}
};
// ----------------------------------------------------------------------------------------------
// Equip
// ----------------------------------------------------------------------------------------------
struct Equip : public ItemData
{	
	int equippos;

	int strength;
	int agility;
	int magic;

	int diangong;
	int huogong;
	int binggong;
	int dugong;

	int diankang;
	int huokang;
	int bingkang;
	int dukang;

	int hp;
	int mp;

	int demandlevel;
	int demandstength;
	int demandagility;
	int demandmagic;

	int mingzhong;
	int baoji;
	int xixue;

	Equip() : ItemData()
	{		
		id = 18;
		img = 4;

		equippos = EQUIP_HEAD;

		strength = 0;
		agility = 0;
		magic = 0;

		diangong = 0;
		huogong = 0;
		binggong = 0;
		dugong = 0;

		diankang = 0;
		huokang = 0;
		bingkang = 0;
		dukang = 0;

		hp = 0;
		mp = 0;

		demandlevel = 0;
		demandstength = 0;
		demandagility = 0;
		demandmagic = 0;

		mingzhong = 0;
		baoji = 0;
		xixue = 0;

	}
	virtual bool use(PlayerData& pd)
	{
		ASSERT(this->equippos < EQUIPNUM);
		if (pd.equipment[equippos])
		{
			Equip* old = (Equip*)ItemDataMgr::inst.findItemData(pd.equipment[equippos]);
			if (old)
				old->unEquip(pd);
		}			
		if(equip(pd))
		{
			event("player", "useequip");
			return true;
		}
		return false;
	}
	virtual bool canEquip(PlayerData& pd)
	{
		//if (pd.level < demandlevel ||
		//	pd.strength < demandstength ||
		//	pd.agility < demandagility ||
		//	pd.magic < demandmagic)
		//	return false;
		return true;
	}
	virtual bool equip(PlayerData& pd)
	{
		if (!canEquip(pd))
			return false;

		pd.strength += strength;
		pd.agility += agility;
		pd.magic += magic;

		pd.diangong += diangong;
		pd.huogong += huogong;
		pd.binggong += binggong;
		pd.dugong += dugong;

		pd.diankang += diankang;
		pd.huokang += huokang;
		pd.bingkang += bingkang;
		pd.dukang += dukang;

		pd.maxhp += hp;
		pd.maxmp += mp;			

		pd.mingzhong += mingzhong;
		pd.baoji += baoji;
		pd.xixue += xixue;

		PRINT("equip " << id)
		PRINT("strength = " << pd.strength)
		PRINT("agility = " << pd.agility)
		PRINT("magic = " << pd.magic)

		PRINT("diangong = " << pd.diangong)
		PRINT("huogong = " << pd.huogong)
		PRINT("binggong = " << pd.binggong)
		PRINT("dugong = " << pd.dugong)

		PRINT("diankang = " << pd.diankang)
		PRINT("huokang = " << pd.huokang)
		PRINT("bingkang = " << pd.bingkang)
		PRINT("dukang = " << pd.dukang)

		PRINT("maxhp = " << pd.maxhp)
		PRINT("maxmp = " << pd.maxmp)

		PRINT("mingzhong = " << pd.mingzhong)
		PRINT("baoji = " << pd.baoji)
		PRINT("xixue = " << pd.xixue)

		return true;
	}
	virtual void unEquip(PlayerData& pd)
	{
		pd.strength -= strength;
		pd.agility -= agility;
		pd.magic -= magic;

		pd.diangong -= diangong;
		pd.huogong -= huogong;
		pd.binggong -= binggong;
		pd.dugong -= dugong;

		pd.diankang -= diankang;
		pd.huokang -= huokang;
		pd.bingkang -= bingkang;
		pd.dukang -= dukang;

		pd.maxhp -= hp;
		pd.maxmp -= mp;

		pd.mingzhong -= mingzhong;
		pd.baoji -= baoji;
		pd.xixue -= xixue;
	}
};

// ----------------------------------------------------------------------------------------------
// Armor
// ----------------------------------------------------------------------------------------------
struct Armor : public Equip
{
	int defence;
	int gedang;

	Armor() : Equip()
	{
		defence = 0;
		gedang = 0;
	}
	virtual bool use(PlayerData& pd)
	{
		if (canEquip(pd))
		{
			ASSERT(this->equippos < EQUIPNUM);
			if (pd.equipment[equippos])
			{
				Armor* old = (Armor*)ItemDataMgr::inst.findItemData(pd.equipment[equippos]);
				if (old)
					old->unEquip(pd);
			}

			equip(pd);
			event("player", "usearmor");
			return true;
		}
		return false;
	}
	virtual void unEquip(PlayerData& pd)
	{
		Equip::unEquip(pd);
	
		pd.defence -= defence;
		pd.gedang -= gedang;
		PRINT("armor unequip " << id);
	}
	virtual bool equip(PlayerData& pd)
	{
		if (Equip::equip(pd))
		{
			pd.defence = defence;
			pd.gedang = gedang;
			PRINT("defence = " << pd.defence)
			PRINT("gedang = " << pd.gedang)

			return true;
		}
		return false;
	}
};
// ----------------------------------------------------------------------------------------------
// Weapon
// ----------------------------------------------------------------------------------------------
struct Weapon : public Equip
{
	int attack;
	real attackinterval;

	Weapon() : Equip()
	{
		equippos = EQUIP_WEAPON;

		attack = 0;
		attackinterval = 0;
	}	
	virtual bool use(PlayerData& pd)
	{
		if (canEquip(pd))
		{
			ASSERT(this->equippos < EQUIPNUM);
			if (pd.equipment[equippos])
			{
				Weapon* old = (Weapon*)ItemDataMgr::inst.findItemData(pd.equipment[equippos]);
				if (old)
					old->unEquip(pd);
			}

			equip(pd);
			event("player", "useweapon");
			return true;
		}
		return false;
	}
	virtual bool equip(PlayerData& pd)
	{
		if (Equip::equip(pd))
		{
			pd.attack = attack;
			pd.attackinterval = attackinterval;
			PRINT("attack = " << pd.attack)
			PRINT("attackinterval = " << pd.attackinterval)
			return true;
		}
		return false;
	}
	virtual void unEquip(PlayerData& pd)
	{
		Equip::unEquip(pd);
		
		pd.attack -= attack;
		pd.attackinterval -= attackinterval;	

		PRINT("weapon unequip " << id);
	}
};


// ----------------------------------------------------------------------------------------------
// MonsterData
// ----------------------------------------------------------------------------------------------
struct MonsterData
{
	int id;
	int img;

	int level;

	int hp;
	int maxhp;
	
	int attack;
	real attackinterval;
	int defence;

	int diankang;
	int huokang;
	int bingkang;
	int dukang;
	
	int diangong;
	int huogong;
	int binggong;
	int dugong;
	int xiangong;
	
	int gedang;
	int mingzhong;
	int baoji;
	int nuqi;

	int huixue;
	int xixue;

	int buzhuo;
	int chixu;
	
	int diaoluo;
	
	int plyerBDmonster;

	MonsterData()
	{
		id = 0;
		img = 0;
		maxhp = 100;
		hp = maxhp;
		attack = 10;
		defence = 3;
		attackinterval = 1;
		diankang = 0;
		huokang = 0;
		bingkang = 0;
		dukang = 0;
		diangong = 0;
		huogong = 0;
		binggong = 0;
		dugong = 0;
		xiangong = 30;
		gedang = 0;
		mingzhong = 80;
		baoji = 0;
		buzhuo = 0;
		chixu = 10;
		huixue = 0;
		xixue = 0;

		diaoluo = 0;
		level = 1;

		nuqi = 0;

		plyerBDmonster = 0;
	}
};
// ----------------------------------------------------------------------------------------------
// MonsterDataMgr
// ----------------------------------------------------------------------------------------------
struct MonsterDataMgr
{	
	std::map<int, MonsterData*> list;
	static MonsterDataMgr inst;
	MonsterData* findItemData(int id)
	{
		std::map<int, MonsterData*>::iterator it = list.find(id);
		if (it != list.end())
		{
			return it->second;		
		}
		return 0;
	}
};
MonsterDataMgr MonsterDataMgr::inst;

// **********************************************************************************************
// Combat
// **********************************************************************************************
int calcPhsicalDamage(int attack, int armor)
{
    real damage = attack;
    if( armor > 0 )
        damage = damage * 1 / (1 + 0.06 * armor);
   
   return (int)damage;
}
// ----------------------------------------------------------------------------------------------
int plyAttack(PlayerData& plydata, MonsterData& monsterdata)
{
	if (plydata.monsterBDplyer > 0)
	{
		if (dx(plydata.monsterBDplyer) == 0)
		{
			plydata.monsterBDplyer = 0; //����Ч���������ʽģ��
		}			
		
		if (dx(12) > 8) //����һ�ֵ�Ӱ��̫�� ����Ӹ��̶����
		{
			PRINT("��ұ�������")
			return 0;
		}
	}

	int skillID = plydata.skillid;
	
	real posionEff = 1;	

	int& plyerHP = plydata.hp;
	int& nuqi = plydata.nuqi;
	int plyerAttack = plydata.getAttck();
	int plyerBJ = plydata.getBaoJi();
	int plyerMZ = plydata.getMingZhong();
	int plyerXX = plydata.xixue;
	int plyerMHP = plydata.getMaxMP();
	int plyerDG = plydata.diangong;
	int plyerBG = plydata.binggong;
	int plyerPG = plydata.dugong;

	int& monsterHP = monsterdata.hp;
	int& monsterGD = monsterdata.gedang;
	int& monsterArmor = monsterdata.defence;
	int monsterDK = monsterdata.diankang;
	int monsterBK = monsterdata.bingkang;
	int monsterPK = monsterdata.dukang;

	int& plyerBDmonster = monsterdata.plyerBDmonster;

	//����Ч��

	//��
	int dianDemage = 0;
	if (plyerDG > monsterDK)
	{
		if (d6 > 3) //�����һ�����ʷ������ݶ�ϵ��
		{
			dianDemage = (plyerDG - monsterDK) * d6; //�ݶ�ϵ��
				monsterHP = monsterHP - dianDemage;
		}
	}

	//��
	if (plyerBG - monsterBK > plyerBDmonster)
	{
		plyerBDmonster = plyerBG - monsterBK;
	}

	//��	
	if (plyerPG > monsterPK)
	{		
		posionEff = 1 + (plyerPG - monsterPK) / 10; //�ݶ�ϵ��
	}

	//������
	if(skillID == 1)
	{
		//ģ������,��
		if( d100 <= plyerMZ && d100 > monsterGD )
		{
			//����������Ч��
			int plyerDamage = calcPhsicalDamage(plyerAttack, monsterArmor);
			//������
			int repeats = 1;
			int dbj = nuqi;
			dbj > 50 ? dbj = 50 : 0;  //��������ŭ�����ֵ����
			int ttbj = plyerBJ + nuqi;
			ttbj > 90 ? ttbj = 90 : 0; //������������
	            
			if(d100 <= ttbj)
			{
				repeats = 2 + int(nuqi / 25);  // �����ݶ�����
			}
			for(int i = 0; i < repeats; i ++)
			{
				//�����Ѫ
				plyerHP = plyerHP + (plyerDamage * plyerXX / 100);
				//���ܳ������Ѫ��
				plyerHP > plyerMHP ? plyerHP = plyerMHP : 0;
				//��������˺�
				monsterHP = monsterHP - plyerDamage * posionEff; //��ϵ��
				//��Ϣ���
				//PRINT("���˴��-��")
				PRINT( "�˹��� �˺�:" << (plyerDamage * posionEff) << "/" << (monsterHP));
				//PRINT(" ŭ��: "<< (nuqi));
	                
				plyerAttack > 0 ? nuqi = 0 : 0;
					
				//KO
				if( monsterHP <= 0)
				{			
					event("monster", "killed");
					PRINT("ɱ�����");
					return 2;
				}
			}		
		}

		//ŭ��ɥʧ
		if( skillID != 2 )
		{
			nuqi > 0 ? nuqi = int(nuqi / 2) : 0;
		}
	}
	return 1;
}
// ----------------------------------------------------------------------------------------------
int monAttack(PlayerData& plydata, MonsterData& monsterdata)
{		
	if (monsterdata.plyerBDmonster > 0)
	{
		if (dx(monsterdata.plyerBDmonster) == 1)
		{
			monsterdata.plyerBDmonster = 0; //����Ч���������ʽģ��
		}

		if (dx(12) > 8) //����һ�ֵ�Ӱ��̫�� ����Ӹ��̶����
		{
			PRINT("���ﱻ������")
			return 0;
		}
	}	
	int plyerSkill = plydata.skillid;

	int& plyerHP = plydata.hp;
	int plyerGD = plydata.getGeDang();
	int plyerArmor = plydata.getDefence();
	int plyerHK = plydata.huokang;
	int plyerDK = plydata.dukang;
	int plyerBK = plydata.bingkang;
	int plyerPK = plydata.dukang;
	int& monsterBDplyer = plydata.monsterBDplyer;
	int& nuqi = plydata.nuqi;
	
	int monsterAttack = monsterdata.attack;
	int monsterDG = monsterdata.diangong;
	int monsterBG = monsterdata.binggong;
	int monsterPG = monsterdata.dugong;
	int monsterHG = monsterdata.huogong;
	int monsterBJ = monsterdata.baoji;
	int monsterHP = monsterdata.hp;
	int monsterMZ = monsterdata.mingzhong;
	int monsterXX = monsterdata.xixue;	

	//����Ч��
            
    //��
    int dianDemage = 0;
    if( monsterDG > plyerDK )
	{
        if( d6 > 3 ) //�����һ�����ʷ������ݶ�ϵ��
		{
            dianDemage = (monsterDG - plyerDK) * d6; //�ݶ�ϵ��
            plyerHP = plyerHP - dianDemage;
		}
	}

    //��
    if( monsterBG - plyerBK > monsterBDplyer )
        monsterBDplyer = monsterBG - plyerBK;    
    
    //��
    real posionEff = 1;
    if( monsterPG > plyerPK )
        posionEff = 1 + (monsterPG - plyerPK) / 10; //�ݶ�ϵ��

        
    //��Ҹ�
    if( plyerGD > 80 ) //����һ�� �����޵�
        plyerGD = 80;
  
    
    //ģ�⹥�����Ч��
    for(int j = 0; j < 1; j ++)
	{
        if( d100 > plyerGD && d100 <= monsterMZ )
		{
            //������
            int repeats = 1;
            if( d100 <= monsterBJ )
                repeats = 2; // �����ݶ�����
            
            for(int ii = 0; ii < repeats; ii ++)
			{
                //����������Ч��
				int monsterDamage = calcPhsicalDamage(monsterAttack, plyerArmor);
                //��Ҽ����˺�
                plyerHP = plyerHP - monsterDamage * posionEff;
                //��Ѫ
                monsterHP = monsterHP + (monsterDamage * monsterXX / 100.0);

				//��Ϣ���
				//PRINT("$�ִ���-��");
				PRINT("$�ֹ��� �˺�:" << (monsterDamage * posionEff) << "/" << (plyerHP));
				//PRINT(" ŭ��: " << (nuqi));

                //KO
				if (plyerHP <= 0)
				{
					event("player", "killed");
					PRINT("��ұ�ɱ��");
					return 2;
				}                    
        
                //����ŭ��ֵ
				if( plyerSkill == 2 )
				{
					nuqi = nuqi + 10 + dx(40); //����ŭ��

					if( nuqi > 100 )
						nuqi = 100;
				}
			}
		}
	}
	plyerHP < 0 ? plyerHP = 0 : 0;

    //KO
	if (plyerHP <= 0)
	{
		PRINT("��ұ�ɱ��");
		return 2;
	}

	return 1;
}
// ----------------------------------------------------------------------------------------------
void fireAttack(PlayerData& plydata, MonsterData& monsterdata)
{
	//��������
	if (monsterdata.huogong > plydata.huokang)
		plydata.hp -= (monsterdata.huogong - plydata.huokang);
	//��һ𹥹���
	if (plydata.huogong > monsterdata.huokang)
		monsterdata.hp -= (plydata.huogong - monsterdata.huokang);

	PRINT("fireAttack");
}


// **********************************************************************************************
// Load data from files
// **********************************************************************************************
bool loadArmorData(const char* filename)
{	
	FILE *fp;

	if (!(fopen_s(&fp, filename, "r")))
	{
		PRINT("file not exist! " << filename);
		return 0;
	}

	Armor armor;
	char name[128];
	while (fscanf_s(fp, "%d\t%s\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n",

		&armor.id,
		name,
		&armor.equippos,
		&armor.strength,
		&armor.agility,
		&armor.magic,
		&armor.diangong,
		&armor.huogong,
		&armor.binggong,
		&armor.dugong,
		&armor.diankang,
		&armor.huokang,
		&armor.bingkang,
		&armor.dukang,
		&armor.hp,
		&armor.mp,
		&armor.demandlevel,
		&armor.demandstength,
		&armor.demandagility,
		&armor.demandmagic,
		&armor.mingzhong,
		&armor.baoji,
		&armor.xixue,
		&armor.defence,
		&armor.gedang,
		&armor.price

		) == 26)
	{
		Armor* am = new Armor;
		memcpy(am, &armor, sizeof(Armor));
		ItemDataMgr::inst.list[am->id] = am;
	}
	fclose(fp);

	PRINT("loadArmorData " << ItemDataMgr::inst.list.size());
	return 1;
}
// ----------------------------------------------------------------------------------------------
bool loadWeaponData(const char* filename)
{
	FILE *fp;

	if (!(fopen_s(&fp, filename, "r")))
	{
		PRINT("file not exist! " << filename);
		return 0;
	}

	Weapon weapon;
	char name[128];
	while (fscanf_s(fp, "%d\t%s\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%f\t%d\n",
		&weapon.id,
		name,
		&weapon.equippos,
		&weapon.strength,
		&weapon.agility,
		&weapon.magic,
		&weapon.diangong,
		&weapon.huogong,
		&weapon.binggong,
		&weapon.dugong,
		&weapon.diankang,
		&weapon.huokang,
		&weapon.bingkang,
		&weapon.dukang,
		&weapon.hp,
		&weapon.mp,
		&weapon.demandlevel,
		&weapon.demandstength,
		&weapon.demandagility,
		&weapon.demandmagic,
		&weapon.mingzhong,
		&weapon.baoji,
		&weapon.xixue,
		&weapon.attack,
		&weapon.attackinterval,
		&weapon.price

		) == 26)
	{
		Weapon* pd = new Weapon;
		memcpy(pd, &weapon, sizeof(Weapon));
		ItemDataMgr::inst.list[pd->id] = pd;
	}
	fclose(fp);

	PRINT("loadWeaponData " << ItemDataMgr::inst.list.size());
	return 1;
}
// ----------------------------------------------------------------------------------------------
bool loadOreData(const char* filename)
{
	FILE *fp;

	if (!(fopen_s(&fp, filename, "r")))
	{
		PRINT("file not exist! " << filename);
		return 0;
	}

	Ore ore;
	char name[128];
	while (fscanf_s(fp, "%d\t%s\t%d\t%d\t%d\n",
		&ore.id,
		name,
		&ore.tichun,
		&ore.tichunnum,
		&ore.price
		) == 5)
	{
		ore.img = 4;
		Ore* pd = new Ore;		
		memcpy(pd, &ore, sizeof(Ore));
		ItemDataMgr::inst.list[pd->id] = pd;
	}
	fclose(fp);

	PRINT("loadOreData " << ItemDataMgr::inst.list.size());
	return 1;
}
// ----------------------------------------------------------------------------------------------
bool loadMonsterData(const char* filename)
{
	FILE *fp;

	if (!(fopen_s(&fp, filename, "r")))
	{
		PRINT("file not exist! " << filename);
		return 0;
	}

	MonsterData mst;
	char name[128];
	while (fscanf_s(fp, "%d\t%s\t%d\t%d\t%d\t%f\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n",
		
		&mst.id,
		name,
		&mst.maxhp,
		&mst.attack,
		&mst.defence,
		&mst.attackinterval,
		&mst.diankang,
		&mst.huokang,
		&mst.bingkang,
		&mst.dukang,
		&mst.diangong,
		&mst.huogong,
		&mst.binggong,
		&mst.dugong,
		&mst.xiangong,
		&mst.gedang,
		&mst.mingzhong,
		&mst.baoji,
		&mst.buzhuo,
		&mst.chixu,
		&mst.huixue,
		&mst.xixue,
		&mst.diaoluo,
		&mst.level
		) == 24)
	{
		MonsterData* md = new MonsterData;
		memcpy(md, &mst, sizeof(MonsterData));
		md->hp = md->maxhp;
		MonsterDataMgr::inst.list[md->id] = md;
	}
	fclose(fp);

	PRINT("loadMonsterData " << MonsterDataMgr::inst.list.size());
	return 1;
}
// ----------------------------------------------------------------------------------------------
const char* getPath()
{
	return "./Life_Data/Plugins";
}

// ----------------------------------------------------------------------------------------------

void initStageDataLogic()
{
	const char* path = getPath();

	{// load item
		{
			std::stringstream ss; ss << path << "/data/" << "armordata.txt";
			loadArmorData(ss.str().c_str());
		}
		{
			std::stringstream ss; ss << path << "/data/" << "weapondata.txt";
			loadWeaponData(ss.str().c_str());
		}
		{
			std::stringstream ss; ss << path << "/data/" << "oredata.txt";
			loadOreData(ss.str().c_str());
		}
		ItemDataMgr::inst.list[100] = new HPMed();
		ItemDataMgr::inst.list[101] = new MPMed();
	}
	
	{// load monster
		std::stringstream ss; ss << path << "/data/" << "monsterdata.txt";
		loadMonsterData(ss.str().c_str());
	}

}
// **********************************************************************************************
// bussniss
// **********************************************************************************************
bool sellitem(PlayerData& pd, int item)
{
	ItemData* id = ItemDataMgr::inst.findItemData(item);
	if (!id)
		return false;
	int dmoney = id->price * rrnd(0.5, 1);
	pd.money += dmoney;// �ۿ�

	PRINT("sell item: " << " id = " << item << " money = " << dmoney)
	return true;
}
//
//// **********************************************************************************************
//// Timer
//// **********************************************************************************************
//struct Timer
//{
//	bool bstart;
//	real timeesp;
//	real interval;
//	int id;
//	bool bloop;
//
//	int monsterid;
//	Timer(real _interval = 1)
//	{
//		bstart = 1;
//		timeesp = 0;
//		interval = _interval;
//		bloop = 0;
//		static int sid = 1;
//		id = sid++;
//	}
//	bool update(real dtime)
//	{
//		if (bstart)
//		{
//			timeesp += dtime;
//			if (timeesp > interval)
//			{
//				PRINT("Timer::update " << id)
//					timeesp = 0;
//				//event("timer", "pulse");
//				trigger();
//				if (!bloop)
//				{
//					return false;
//				}
//			}
//		}
//		return true;
//	}
//	virtual void start()
//	{
//		PRINT("Timer::start " << id)
//			bstart = 1;
//		trigger();
//	}
//	virtual void end()
//	{
//		PRINT("Timer::end " << id)
//			bstart = 0;
//	}
//	virtual void trigger()
//	{
//		PRINT("Timer::trigger " << id)
//	}
//};
//std::vector<Timer*>	timers;
//
//void addtimer(Timer* timer)
//{
//	timers.push_back(timer);
//}
//Timer* gettimer(int ind)
//{
//	ASSERT(timers.size() > ind);
//	return timers[ind];
//}
//void updatetimers()
//{
//	static real soldgtime = gtime;
//	std::vector<Timer*>::iterator it = timers.begin();
//	while (it != timers.end())
//	{
//		if (!(*it)->update(gtime - soldgtime))
//		{
//			delete (*it);
//			it = timers.erase(it);
//			continue;
//		}
//		++it;
//	}
//	soldgtime = gtime;
//}