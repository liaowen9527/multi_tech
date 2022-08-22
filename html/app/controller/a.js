(function (app) {
    'use strict';

    angular.module('nb').controller('nbACtrl', nbAController);
    
    nbAController.$inject = ['$scope'];
    function nbAController($scope) {
        var self = this;
        var scope = $scope;

        self.data = $scope.data;
        self.data.firstName = self.data.firstName || "a";
        self.data.lastName = self.data.lastName || "b";

        self.getFullName = function() {
            return scope.data.firstName + " " + scope.data.lastName;
        }

        self.getData = function () {
            return scope.data;
        }
    }

})(NetBrainApp);