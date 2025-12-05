# Traits Class 类

### Traits Class 工具箱
* **Traits Class** 是啥呢，为什么要了解这个或者说为什么要用 traits？
  * 在我拿到公司某模块的开发权限之前，我想我也不会想到这个 **技术**。是的，**Traits Class** 其实只是一种技术，只是一种约定俗成的模板编程技术，不是什么很抽象的概念。如果代码中含有过多函数重载，或者说根本不是 "is-a" 的继承，而刚好又有需求要求你改造这部分功能。那么恭喜你，你可能真的得了解什么是 **Traits** 了，毕竟在迭代器的设计选择中大名鼎鼎的 **STL** 都是用的 **Traits** 而不是继承
  * Traits 可以理解为一个工具箱，它给出一个基础模板，里面包含一类事物的基础特性，你可以在这个基础模板上做一些特例化，之后在代码编写中，根据特例化操作我们就可以做一些特定的动作。
    * 这里以材料 *Material* 为例（你也许难以完全理解 《effective C++》 里面 iterator 的例子，毕竟作为例子它或许全面，但有点抽象并过于冗长了，这里以实际能接触到的具象化事物举例），它有一些基础特性：密度（默认为 1.0)、强度（1.0）、形态（固液气，默认为固态）...我们想用一个通用函数来计算一个物体的重量：体积 * 密度。但因为不同材料的密度不同，所以我将以 traits 为可能用到的每种材料提供密度，特别的，为了不让这个模板函数太单调，可以加一下简单的描述类函数：
      * 定义基础材料特性模板类（traits），默认情况下密度为 1；
      * 为每种材料特例化这个 traits，提供密度等特性；
      * 编写一个计算重量的函数模板，其中使用材料 traits 获取密度。
    * 我们这里可以用 Wood、Glass、Metal 作为 Material 来举例：
      ```cpp
        #include <iostream>
        #include <string>

        // ==================== 基础定义 ====================
        class Wood {};
        class Glass {};
        class Metal {};
        class Plastic {};

        // ==================== 材料特性模板 ====================
        // 默认特性模板 - 适用于大多数材料
        template<typename Material>   // 同 template<typename T> 这里写 Material 只是方便理解，事实上它只是一个代号
        struct material_traits {
            static constexpr const char* name = "Unknown Material";
            static constexpr float density = 1.0f;      // g/cm³
            static constexpr float strength = 1.0f;     // 强度系数
            static constexpr bool is_transparent = false;
            static constexpr bool can_burn = false;
            
            static std::string describe() {
                return "Generic material";
            }
        };

        // ==================== 特化：木头 ====================
        template<>
        struct material_traits<Wood> {
            static constexpr const char* name = "Wood";
            static constexpr float density = 0.6f;      // 木头较轻
            static constexpr float strength = 2.5f;     // 相对强度
            static constexpr bool is_transparent = false;
            static constexpr bool can_burn = true;      // 木头能燃烧
            
            static std::string describe() {
                return "Natural organic material from trees";
            }
            
            // 木材特有的属性
            static constexpr bool has_grain = true;
            static constexpr const char* grain_type = "varies";
        };

        // ==================== 特化：玻璃 ====================
        template<>
        struct material_traits<Glass> {
            static constexpr const char* name = "Glass";
            static constexpr float density = 2.5f;      // 玻璃较重
            static constexpr float strength = 0.8f;     // 易碎
            static constexpr bool is_transparent = true; // 透明
            static constexpr bool can_burn = false;     // 玻璃不燃烧
            
            static std::string describe() {
                return "Brittle transparent material made from silica";
            }
            
            // 玻璃特有的属性
            static constexpr float refractive_index = 1.5f;
            static constexpr bool is_breakable = true;
        };

        // ==================== 特化：金属 ====================
        template<>
        struct material_traits<Metal> {
            static constexpr const char* name = "Metal";
            static constexpr float density = 7.8f;      // 很重
            static constexpr float strength = 10.0f;    // 很强
            static constexpr bool is_transparent = false;
            static constexpr bool can_burn = false;
            
            static std::string describe() {
                return "Hard, shiny material that conducts electricity";
            }
            
            // 金属特有的属性
            static constexpr bool conducts_electricity = true;
            static constexpr bool is_magnetic = false;
        };

        // ==================== 使用特性的工厂函数 ====================
        template<typename Material>
        class FurniturePiece {
        private:
            float volume;  // 立方厘米
            
        public:
            FurniturePiece(float vol) : volume(vol) {}
            
            // 计算重量
            float calculate_weight() const {
                // 使用 traits 中的密度
                return volume * material_traits<Material>::density;
            }
            
            // 生成描述
            std::string get_description() const {
                return std::string(material_traits<Material>::name) + 
                    " furniture, weight: " + std::to_string(calculate_weight()) + "g";
            }
            
            // 安全检查
            bool is_fire_hazard() const {
                return material_traits<Material>::can_burn;
            }
            
            // 透明度检查
            bool is_see_through() const {
                return material_traits<Material>::is_transparent;
            }
        };

        // ==================== 处理函数（使用 traits） ====================
        template<typename Material>
        void process_material() {
            std::cout << "\n=== Processing " << material_traits<Material>::name << " ===" << std::endl;
            std::cout << "Description: " << material_traits<Material>::describe() << std::endl;
            std::cout << "Density: " << material_traits<Material>::density << " g/cm³" << std::endl;
            std::cout << "Strength: " << material_traits<Material>::strength << std::endl;
            std::cout << "Transparent: " << (material_traits<Material>::is_transparent ? "Yes" : "No") << std::endl;
            std::cout << "Flammable: " << (material_traits<Material>::can_burn ? "Yes" : "No") << std::endl;
        }

        // ==================== 实际应用 ====================
        int main() {
            // 1. 显示不同材料的特性
            process_material<Wood>();
            process_material<Glass>();
            process_material<Metal>();
            
            std::cout << "\n=== Furniture Examples ===" << std::endl;
            
            // 2. 创建不同材料的家具
            FurniturePiece<Wood> wooden_table(5000);  // 5000 cm³ 的木头桌子
            FurniturePiece<Glass> glass_table(3000);   // 3000 cm³ 的玻璃桌子
            FurniturePiece<Metal> metal_chair(2000);   // 2000 cm³ 的金属椅子
            
            // 3. 显示家具信息
            std::cout << wooden_table.get_description() << std::endl;
            std::cout << "Fire hazard: " << (wooden_table.is_fire_hazard() ? "Yes" : "No") << std::endl;
            
            std::cout << glass_table.get_description() << std::endl;
            std::cout << "See-through: " << (glass_table.is_see_through() ? "Yes" : "No") << std::endl;
            
            std::cout << metal_chair.get_description() << std::endl;
            std::cout << "Fire hazard: " << (metal_chair.is_fire_hazard() ? "Yes" : "No") << std::endl;
            
            // 4. 编译时判断
            std::cout << "\n=== Compile-time Checks ===" << std::endl;
            
            // 这些特性在编译时就确定了
            if constexpr (material_traits<Wood>::can_burn) {
                std::cout << "Warning: Wood furniture requires fire safety measures!" << std::endl;
            }
            
            if constexpr (material_traits<Glass>::is_transparent) {
                std::cout << "Tip: Glass furniture needs regular cleaning to stay clear!" << std::endl;
            }
            
            return 0;
        }
      ```
    * 如上述代码所述，get_description() 完成了名字输出 + 重量计算，使用者无需知道具体实现，直接调用函数即可得到每种材料的简要描述。而且最重要的一点是，这一切都发生在编译期，这意味着不会像继承那样在运行期判断类型造成性能损失。同时，因为是编译期确定信息，可以有效防止一些意料之外的事情发生，比如说编译器可以确保我们不会对 Glass 这种材料使用 has_grain() 函数。
    * 那么也就是说，你可以把 Traits 当作这个材料的规格说明书：
      * 定义默认模板： **material_traits** 是通用材料的说明书；
      * 定义特化版本： **material_traits<Wood/Glass/Metal>** 为特定材料的说明书；
      * 你可以用你自己定义的工厂类（**FurniturePiece**）或者模板函数（**process_material**）去调用这个 traits，达到静态多态的目的。

### 优势
* 如上述代码所述，这个东西我觉得最大的优势是 **它在编译期就决定了相关类型及该类型特定的调用方法，能有效提高运行期的效率**；
* **能实现静态多态**，区别于虚函数继承实现动态多态；
    * **Q:** 那既然是静态多态，我用函数重载不也一样吗？
    * **A:** 这就是我接手代码想要优化的主要原因，如果 10 个类型要用到 10 个函数，那你就要编写 100 个类似函数，而 traits 只要特例化 10 个；
* **可以不改动源码即可直接添加新特性**。Traits 更像是一个跳板，你只需另外定义一个使用类，并对特化你所需的类型即可实现，有很好的扩展性。
* **可以使内置类型也可享用你的特制函数**，这也是继承做不到的；上述例子不能表现这点，以下我将以数组来解释：
  * 我们有一个需求，打印一个容器的个数，则：
    ```cpp
    template<typename T>
    void printSize(const T& container) {
        size_t size = container.size();
    }
    ```
    这里咋一看没有问题，vector string stack 不都有 .size() 吗，但可惜的是普通数组并没有。
  * 想要解决这个问题，你可以引入一个 traits：
    ```cpp
    // 创建 Traits 默认模板
    templace <typename T>
    struct ContainerTraits {
        static const bool has_size_method = false;    // 同内置类型，默认没有 size() 函数
    }

    // 为 vector 特例化此模板
    template<typename T>
    struct ContainerTraits<std::vector<T>> {
        static const bool has_size_method = true;
        static size_t getSize(const std::vector<T>& v) {
            return v.size();
        }
    }

    // 为数组特例化
    template<typename T, size_t N>
    struct ContainerTraits<T[N]> {
        static const bool has_size_method = true;
        static size_t getSize(const T (&arr)[N]) {
            return N;
        }
    }
    ```
  * 由此，我们便可对内置数组使用 getsize 函数了：
    ```cpp
    template<typename T>
    void safePrintSize(const T& c) {
        if constexpr (ContainerTraits<T>::has_size_method) {
            std::cout << ContainerTraits<T>::getSize(c) << "\n";
        }
        else {
            std::cout << "Unknown type" << "\n";
        }
    }
    ```

### 结语
* 这里我想引用 《effective C++》第三版中一句话来结束：Traits classes 使得 “类型相关信息” 在编译器可用。它们以 templates 和 “templates 特化” 来完成实现。