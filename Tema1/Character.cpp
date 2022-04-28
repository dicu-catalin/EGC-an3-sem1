//#include "lab_m1/Tema1/Character.h"
//#include "lab_m1/Tema1/transform2D.h"
//#include "lab_m1/Tema1/object2D.h"
//
//#include <vector>
//#include <iostream>
//
//
//using namespace std;
//using namespace c1;
//
//Character::Character()
//{
//
//}
//
//Character::Character(glm::vec3 leftBottomCorner, float length, glm::vec3 color, float tx, float ty) :
//    corner(leftBottomCorner), length(length), color(color), tx(tx), ty(ty)
//{
//
//}
//
//void Character::CreateMeshes()
//{
//    std::vector<VertexFormat> vertices =
//    {
//        VertexFormat(corner, color),
//        VertexFormat(corner + glm::vec3(length, 0, 0), color),
//        VertexFormat(corner + glm::vec3(length, length, 0), color),
//        VertexFormat(corner + glm::vec3(0, length, 0), color)
//    };
//
//    Mesh* character = new Mesh("square1");
//    std::vector<unsigned int> indices = { 0, 1, 2, 3, 0, 2 };
//
//    character->InitFromData(vertices, indices);
//    Mesh* square1 = object2D::CreateCharacter("square1", corner, length, glm::vec3(1, 0, 0), true);
//    AddMeshToList(character);
//}
//
//void Character::RenderMeshes(glm::mat3 visMatrix, float angularstep)
//{
//    glm::mat3 modelMatrix;
//    modelMatrix = visMatrix * transform2D::Translate(tx, ty);
//    modelMatrix *= transform2D::Translate(length / 2, length / 2) * transform2D::Rotate(angularstep)
//        * transform2D::Translate(-length / 2, -length / 2);
//    RenderMesh2D(meshes["square1"], shaders["VertexColor"], modelMatrix);
//}
//
//void Character::SetTxTy(float tx, float ty)
//{
//    this->tx += tx;
//    this->ty += ty;
//}