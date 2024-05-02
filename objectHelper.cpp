#include <iostream>
#include "Object3D.h"

//
// Created by Matthew Halderman on 5/1/24.
//
class objectHelper {
    static void rotateChildren(Object3D& model) {
        //std::cout << "Model Children: " << model.numberOfChildren() << "\n";
        for (int i = 0; i < model.numberOfChildren(); i++) {
            auto &child = model.getChild(i);
            child.rotate(glm::vec3(0, 0.1, 0));
            objectHelper::rotateChildren(child);
        }
    }
};
