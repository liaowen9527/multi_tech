(function (app) {
    'use strict';

    angular.module('nb').controller('nbCCtrl', nbAController);
    
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
        self.onExport = function (ele_scope, param) {
            var a = 1;
            a = 2;
        }
    }

})(NetBrainApp);